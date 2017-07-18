#ifndef integrator_hpp
#define integrator_hpp
#include <iostream>
#include <eigen3/Eigen/Dense>
#include "spacecraft.hpp"

void propogate(
  Spacecraft::State & state,
  const Spacecraft::Control & control,
  const double & t,
  const double & abstol = 1e-10,
  const double & reltol = 1e-10,
  const double & maxiter = 10000,
  const double & maxorder = 3000
) {

  double xm;
  double em;

  for (int i=0; i<2; i++) {

    // First, we compute the order pm for the Taylor method as follows: we define εm as
    xm = state.lpNorm<Eigen::Infinity>();

    // and then,
    (reltol*xm < abstol) ? em = abstol : em = reltol;
    int order = (int)(ceil(-0.5*log(em) + 1));
    // make sure poly order is not too high



  };

};
#endif
