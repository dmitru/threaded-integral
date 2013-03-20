    
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "integral.h"

double f(double x)
{
  return x*x + x + 1.38 / (x + 0.1);
}

int main(int argc, char **argv)
{
  if (argc < 3) {
    fprintf(stderr, "Usage: %s <number of threads> <number of points>\n", 
      argv[0]);
    exit(1);
  }

  int n_threads = atoi(argv[1]);
  int n_points = atoi(argv[2]);

  double ans;
  if (!integrate(f, 0.0, M_PI, n_threads, n_points, &ans)) {
    printf("%.10f\n", ans); 
  } else {
    fprintf(stderr, "Error: something went wrong when calling integrate()\n");
    exit(2);
  }

  return 0;
} 