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
    const std::vector<double> & x,
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
    const double umag(sqrt(pow(u[0],2) + pow(u[1],2) + pow(u[2],2)));

    // velocity
    dxdt[0] = x[3];
    dxdt[1] = x[4];
    dxdt[2] = x[5];

    //propulsion
    dxdt[3] = u[0]*spacecraft.thrust/x[6];
    dxdt[4] = u[1]*spacecraft.thrust/x[6];
    dxdt[5] = u[2]*spacecraft.thrust/x[6];
    dxdt[6] = -umag*spacecraft.thrust/spacecraft.veff;

    // gravity
    for (int i=0; i<bodies.size(); ++i) {
      // barycentric state
      std::vector<double> s(bodies[i].state(t));
      // relative position
      for (int j=0; j<3; ++j) {s.at(j) = x.at(j) - s.at(j);};
      // computing cube norm
      const double r3(pow(sqrt(pow(s[0],2) + pow(s[1],2) + pow(s[2],2)), 3));
      // add influence
      for (int j=0, k=3; j<3, k<6; ++j, ++k) {
        dxdt.at(k) -= bodies[i].mu*s.at(j)/r3;
      };
    };
    return dxdt;
  };



};


#endif
