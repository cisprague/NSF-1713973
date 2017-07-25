// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp
// NSF Award 1713973

#ifndef dynamics_hpp
#define dynamics_hpp
#include <vector>
#include "spacecraft.hpp"
#include "body.hpp"
#include "linalg.hpp"

namespace Dynamics {

  std::vector<double> ODE (
    const std::vector<double> & x,
    const std::vector<double> & u,
    const double              & t,
    const std::vector<Body>   & bodies,
    const Spacecraft          & spacecraft
  ) {

    // allocate first order system
    std::vector<double> dxdt(7);

    // compute metrics
    const std::vector<double> F(spacecraft.force(u)); // force vector
    const double mdot(spacecraft.mdot(u));            // mass flow rate

    // velocity
    dxdt[0] = x[3];
    dxdt[1] = x[4];
    dxdt[2] = x[5];

    // propulsion
    dxdt[3] = F[0]/x[6];
    dxdt[4] = F[1]/x[6];
    dxdt[5] = F[2]/x[6];
    dxdt[6] = mdot;

    // gravity
    for (int i=0; i<bodies.size(); ++i) {
      // barycentric body position
      std::vector<double> r(bodies[i].position(t));
      // sc position wrt body
      r[0] = x[0] - r[0];
      r[1] = x[1] - r[1];
      r[2] = x[2] - r[2];
      // computing cube norm
      double r3(linalg::normpow(r, 3));
      dxdt[3] -= bodies[i].mu*r[0]/r3;
      dxdt[4] -= bodies[i].mu*r[1]/r3;
      dxdt[5] -= bodies[i].mu*r[2]/r3;
    };
    return dxdt;
  };

  struct Ballistic {

    // members
    const std::vector<Body>   bodies;
    const Spacecraft          spacecraft;

    // constructor
    Ballistic (
      const std::vector<Body> & bodies_,
      const Spacecraft        & spacecraft_
    ) : bodies(bodies_), spacecraft(spacecraft_) {};

    // destructor
    ~Ballistic (void) {};

    // dynamics
    void operator () (
      const std::vector<double> & x,
      std::vector<double>       & dxdt,
      const double              & t
    ) const {
      // we set the control to zero
      const std::vector<double> u(3,0);
      dxdt = ODE(x, u, t, bodies, spacecraft);
    };
  };

  struct Constant_Control : public Ballistic {

    // control
    const std::vector<double> control;

    // constructor
    Constant_Control (
      const std::vector<Body>   & bodies_,
      const Spacecraft          & spacecraft_,
      const std::vector<double> & control_
    ) : Ballistic(bodies_, spacecraft_), control(control_) {};

    // destructor
    ~Constant_Control (void) {};

    void operator () (
      const std::vector<double> & x,
      std::vector<double>       & dxdt,
      const double              & t
    ) const {
      dxdt = ODE(x, control, t, bodies, spacecraft);
    };
  };


};

#endif
