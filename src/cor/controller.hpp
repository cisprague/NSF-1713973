// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp

#ifndef controller_hpp
#define controller_hpp
#include <cmath>
#include <iostream>
#include "mlp.hpp"
#include "linalg.hpp"

namespace Controller {

  struct Base {

    // neural network
    ML::MLP mlp;

    // constructor
    Base (
      const int & nin_,
      const std::vector<int> & hshape_,
      const std::vector<double> & refs_
    ) : mlp(ML::MLP::full_struct(hshape_, nin_, 3), refs_) {};

    // destructor
    ~Base (void) {};

    // map state to control
    std::vector<double> operator() (const std::vector<double> & x) {

      // we compute the neural network output
      std::vector<double> u(mlp(x));

      // compute unit vector parametres
      const double mag(u[0]);          // u ∈ [0, 1]
      const double theta(u[1]*2*M_PI); // θ ∈ [0, 2π]
      const double z(u[2]*2 - 1);      // z ∈ [-1, 1]

      // compute the thrust vector
      return linalg::scaled_unit_vector(mag, theta, z);
    };

  };

  struct Temporal : public Base {

    // constructor
    Temporal (
      const std::vector<int> & hshape_,
      const std::vector<double> & refs_
    ) : Base(7, hshape_, refs_) {};

    // destructor
    ~Temporal () {};

  };

  struct Relative : public Base {

    // constructor
    Relative (
      const int & nbod_,
      const std::vector<int> & hshape_,
      const std::vector<double> & refs_
    ) : Base(nbod_*6 + 1, hshape_, refs_) {};

    // destructor
    ~Relative (void) {};

  };

};

#endif
