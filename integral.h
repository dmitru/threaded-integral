
#ifndef INTEGRAL_H
#define INTEGRAL_H

int integrate(double (*f)(double), double a, double b, 
  int n_threads, int n_points, double *res);

#endif  // INTEGRAL_H o