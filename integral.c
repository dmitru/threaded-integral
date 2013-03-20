
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#include "integral.h"

struct Task {
  double a;
  double b;
  int n_points;
  double (*f)(double);
  double *ans;
};

typedef struct Task Task;

void* thread_integrate(void *arg)
{
  Task *task = (Task*)arg;
  double a = task->a;
  double b = task->b;
  int n_points = task->n_points;
  double (*f)(double) = task->f;
  double* ans = task->ans;

  double res = 0.0;
  double delta = (b - a) / n_points;
  int i = 0;
  for (double x = a; i < n_points - 1; ++i, x += delta) {
    double y1 = f(x);
    double y2 = f(x + delta);
    res += delta * (y2 + y1);
  }

  *ans = res / 2.0;

  return NULL;
}

int integrate(double (*f)(double), double a, double b, 
  int n_threads, int n_points, double *res)
{
  if (n_points < n_threads * 2) {
    return -2;
  }

  pthread_t *threads_handles = (pthread_t*) malloc(n_threads * sizeof(pthread_t));
  if (threads_handles == NULL) {
    return -1;
  }

  double *ans = (double*) malloc(n_threads * sizeof(double));
  if (ans == NULL) {
    free(threads_handles);
    return -1;
  }

  Task* tasks = (Task*) malloc(n_threads * sizeof(Task));
   if (ans == NULL) {
    free(threads_handles);
    free(ans);
    return -1;
  }

  double delta = (b - a) / n_points;
  int points_per_thread = 1 + (n_points - 1) / n_threads;
  for (int i = 0; i < n_threads; ++i) {
    int first_point = i * points_per_thread;
    int last_point = first_point + points_per_thread - 1;
    if (last_point > n_points)
      last_point = n_points - 1;
    tasks[i].a = delta * first_point;
    tasks[i].b = delta * last_point;
    tasks[i].n_points = last_point - first_point + 1;
    tasks[i].ans = &ans[i];
    tasks[i].f = f;
    int create_status = pthread_create(&threads_handles[i], NULL, 
      thread_integrate, (void*)&tasks[i]);
    if (create_status) {
      free(threads_handles);
      free(ans);
      return create_status;
    }
  }

  for (int i = 0; i < n_threads; ++i) {
    int join_status = pthread_join(threads_handles[i], NULL);
    if (join_status) {
      free(threads_handles);
      free(ans);
      return join_status;
    }
  }

  double t_res = 0.0;
  for (int i = 0; i < n_threads; ++i) {
    t_res += ans[i];
  }
  *res = t_res;

  return 0;
}