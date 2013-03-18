
#ifndef INTEGRAL_H
#define INTEGRAL_H

float integrate(float (*f)(float), float a, float b, 
  int n_threads, int n_points);

#endif  // INTEGRAL_H 