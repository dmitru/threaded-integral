
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#include "integral.h"

struct Task {
  double a;
  double b;
  int n_points;
  double (*f)(double);
};

typedef struct Task Task;

void* thread_integrate(void *arg)
{
  Task *task = (Task*)arg;
  double a = task->a;
  double b = task->b;
  int n_points = task->n_points;
  double (*f)(double) = task->f;
  free(task);
  double *ans = (double*)malloc(sizeof(double));
  if (!ans)
    return NULL;

  double res = 0.0;
  double delta = (b - a) / n_points;
  int i = 0;
  for (double x = a; i < n_points - 1; ++i, x += delta) {
    double y1 = f(x);
    double y2 = f(x + delta);
    res += delta * (y2 + y1);
  }

  *ans = res / 2.0;

  return ans;
}

int integrate(double (*f)(double), double a, double b, 
  int n_threads, int n_points, double *res)
{
  if (n_points < n_threads * 2 || n_points <= 0 || n_threads <= 0) {
    return 1;
  }

  pthread_t *threads_handles = (pthread_t*) malloc(n_threads * sizeof(pthread_t));
  if (threads_handles == NULL) {
    return 2;
  }

  double delta = (b - a) / n_points;
  int points_per_thread = 1 + (n_points - 1) / n_threads;
  for (int i = 0; i < n_threads; ++i) {
    int first_point = i * points_per_thread;
    int last_point = first_point + points_per_thread - 1;
    if (last_point > n_points)
      last_point = n_points - 1;
    Task* task = (Task*) malloc(sizeof(Task));
    task->a = delta * first_point;
    task->b = delta * last_point;
    task->n_points = last_point - first_point + 1;
    task->f = f;
    int create_status = pthread_create(&threads_handles[i], NULL, 
      thread_integrate, (void*)task);
    if (create_status) {
      free(threads_handles);
      return 3;
    }
  }

  double t_res = 0.0;
  for (int i = 0; i < n_threads; ++i) {
    double *ans;
    int join_status = pthread_join(threads_handles[i], (void**)&ans);
    if (join_status || ans == NULL) {
      free(threads_handles);
      return 4;
    }
    t_res += *ans;
    free(ans);
  }
  *res = t_res;

  free(threads_handles);

  return 0;
}
