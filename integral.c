
#include <pthread.h>

#include "integral.h"

float integrate(float (*f)(float), float a, float b, 
  int n_threads, int n_points)
{
  // TODO: convert it to threaded implementation
  float res = 0.0;
  float delta = (b - a) / n_points;
  for (int i = 0; i < n_points - 1; ++i) {
    float x1 = i * delta;
    float x2 = i * delta + delta;
    float y1 = f(x1);
    float y2 = f(x2);
    res += delta * (y2 + y1) / 2;
  }

  return res;
}