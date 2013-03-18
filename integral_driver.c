
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "integral.h"

float f(float x)
{
  return sin(x);
}

int main(int argc, char **argv)
{
  if (argc < 3) {
    fprintf(stderr, "Usage: %s <number of threads> <number of points>\n", argv[0]);
    exit(1);
  }

  int n_threads = atoi(argv[1]);
  int n_points = atoi(argv[2]);

  float ans = integrate(f, 0.0, M_PI, n_threads, n_points);
  printf("%.10f\n", ans);

  return 0;
} 