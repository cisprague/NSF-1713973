// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp

#ifndef controller_hpp
#define controller
#include <cmath>
#include <iostream>
#include "mlp.hpp"
#include "linalg.hpp"

struct Controller {

  /*
    x  O ---
    y  O --- \
    z  O --- --- O u ∈ [0, 1]
    vx O --- --- O θ ∈ [0, 2π]
    vy O --- --- O z ∈ [-1, 1]
    vz O --- /
    m  O --- /
  */

  // members
  const double thrust;
  ML::MLP mlp;

  // constructor
  Controller (
    const double              & thrust_,
    const std::vector<int>    & hidden_struct_,
    const std::vector<double> & refs_
  ) : thrust(thrust_), mlp(ML::MLP::full_struct(hidden_struct_,7,3),refs_) {};

  // destructor
  ~Controller (void) {};

  // map state to control
  std::vector<double> operator() (const std::vector<double> & x) {

    // we compute the neural network output
    std::vector<double> u(mlp(x));

    // compute unit vector parametres
    const double mag(u[0]*thrust);   // u ∈ [0, thrust]
    const double theta(u[1]*2*M_PI); // θ ∈ [0, 2π]
    const double z(u[2]*2 - 1);      // z ∈ [-1, 1]

    // compute the thrust vector
    return linalg::scaled_unit_vector(mag, theta, z);
  };



};

#endif
