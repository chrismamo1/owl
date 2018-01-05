/*
 * OWL - an OCaml numerical library for scientific computing
 * Copyright (c) 2016-2017 Liang Wang <liang.wang@cl.cam.ac.uk>
 */

#include "owl_maths.h"
#include "owl_stats.h"

/** Beta distribution **/

double beta_rvs(double a, double b)
{
  double Ga, Gb;

  if ((a <= 1.) && (b <= 1.)) {
    double U, V, X, Y;

    while (1) {
      U = sfmt_f64_3;
      V = sfmt_f64_3;
      X = pow(U, 1. / a);
      Y = pow(V, 1. / b);

      if ((X + Y) <= 1.0) {
        if (X +Y > 0)
          return X / (X + Y);
        else {
          double logX = log(U) / a;
          double logY = log(V) / b;
          double logM = logX > logY ? logX : logY;
          logX -= logM;
          logY -= logM;
          return exp(logX - log(exp(logX) + exp(logY)));
        }
      }
    }
  }
  else {
    Ga = std_gamma_rvs(a);
    Gb = std_gamma_rvs(b);
    return Ga / (Ga + Gb);
  }
}

double beta_pdf(double x, double a, double b) {
  if (x < 0 || x > 1)
    return 0;
  else {
    double gab = lgam(a + b);
    double ga = lgam(a);
    double gb = lgam(b);

    if (x == 0.0 || x == 1.0) {
      if (a > 1.0 && b > 1.0)
        return 0.0;
      else
       return exp(gab - ga - gb) * pow(x, a - 1) * pow(1 - x, b - 1);
    }
    else
      return exp(gab - ga - gb + log(x) * (a - 1)  + log1p(-x) * (b - 1));
    }
}

double beta_logpdf(double x, double a, double b) {
  return log(beta_pdf(x, a, b));
}

double beta_cdf(double x, double a, double b) {
  return btdtr(a, b, x);
}

double beta_logcdf(double x, double a, double b) {
  return log(beta_cdf(x, a, b));
}

double beta_ppf(double q, double a, double b) {
  return incbi(a, b, q);
}