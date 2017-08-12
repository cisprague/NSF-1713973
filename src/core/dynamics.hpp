#ifndef dynamics_hpp
#define dynamics_hpp
#include <vector>
#include "spacecraft.hpp"
#include "body.hpp"
#include "controller.hpp"
#include "constants.hpp"

struct Dynamics {

  // members
  const Spacecraft spacecraft;
  const std::vector<Body> bodies;
  const Controller controller;

  // constructor
  Dynamics (
    const Spacecraft & spacecraft_,
    const std::vector<Body> & bodies_,
    const Controller & controller_
  ) :
    spacecraft(spacecraft_),
    bodies(bodies_),
    controller(controller_) {
  };

  // destructor
  ~Dynamics (void) {};

  // compute rate
  std::vector<double> operator () (
    const std::vector<double> x,
    std::vector<double> & dxdt,
    const double & t
  ) const {

    // check state validity
    if (x.size() != 7) {throw "State must be 7D.";};
    // check rate validity
    if (dxdt.size() != 7) {throw "dxdt must be 7D.";};

    // compute control throttle
    const std::vector<double> u(controller(x, t));
    // throttle magnitude
    const double umag(sqrt(pow(u.at(0),2) + pow(u.at(1),2) + pow(u.at(2),2)));

    // NOTE: non-dimensionalise these things

    // velocity
    dxdt.at(0) = x.at(3);
    dxdt.at(1) = x.at(4);
    dxdt.at(2) = x.at(5);

    //propulsion
    dxdt.at(3) = u.at(0)*spacecraft.thrust/x.at(6);
    dxdt.at(4) = u.at(1)*spacecraft.thrust/x.at(6);
    dxdt.at(5) = u.at(2)*spacecraft.thrust/x.at(6);
    dxdt.at(6) = umag*spacecraft.thrust/spacecraft.veff;

    // gravity
    for (int i=0; i<bodies.size(); ++i) {
      // barycentric state
      std::vector<double> s(bodies.at(i).state(t));
      // relative position
      for (int j=0; j<3; ++j) {s.at(j) = x.at(j) - s.at(j);};
      // computing cube norm
      const double r3(pow(sqrt(pow(s.at(0),2) + pow(s.at(1),2) + pow(s.at(2),2)), 3));
      // add influence
      for (int j=0, k=3; j<3, k<6; ++j, ++k) {
        dxdt.at(k) -= bodies.at(i).mu*s.at(j)/r3;
      };
    };
    return dxdt;
  };



};


#endif
