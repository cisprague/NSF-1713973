// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp

#ifndef controller_hpp
#define controller_hpp
#include <cmath>
#include <iostream>
#include "mlp.hpp"
#include "linalg.hpp"
#include "body.hpp"


// controllers
namespace Controller {

  struct Base {

    // neural network
    ML::MLP mlp;

    // constructor
    Base (
      const int                 & nin_,
      const std::vector<int>    & hshape_,
      const std::vector<double> & refs_
    ) : mlp(nin_, 3, hshape_, refs_) {};

    // destructor
    ~Base (void) {};

    // map state to control
    std::vector<double> operator () (
      const std::vector<double> & x,
      const double              & t = 0
    ) {

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
    ~Temporal (void) {};

  };

  struct Relative : public Base {

    // members
    const std::vector<Body> bodies;
    const int nbod;
    const int ni;

    // constructor
    Relative (
      const std::vector<Body>   & bodies_,
      const std::vector<int>    & hshape_,
      const std::vector<double> & refs_
    ) :
      bodies(bodies_),
      nbod(bodies.size()),
      ni(nbod*6 + 1),
      Base(bodies_.size()*6 + 1, hshape_, refs_) {};

    // destructor
    ~Relative (void) {};

    // map state to control
    std::vector<double> operator () (
      const std::vector<double> & x,
      const double & t
    ) {

      // the neural network input vector
      std::vector<double> in(ni);

      // we compute the relative states
      for (int i=0; i<nbod; ++i) {
        // barycentric state of body
        const std::vector<double> xb(bodies.at(i).state(t));
        for (int j=0; j<6; ++j) {
          // relative state of spacecraft
          in.at(i*6+j) = x.at(i) - xb.at(i);
        };
      };

      // last node for mass
      in.at(ni-1) = x.at(6);

      return Base::operator()(in);
    };

  };

};

#endif
