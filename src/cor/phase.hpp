#ifndef phase_hpp
#define phase_hpp
#include <eigen3/Eigen/Dense>
#include <iostream>
#include <random>
#include <vector>
#include <boost/numeric/odeint.hpp>
#include <stdexcept>
#include "body.hpp"
#include "spacecraft.hpp"

struct Phase {

  /*
    zi = {t0,tN,ux0,uy0,uz0,...,uxM,uyM,uzM}
    i = phase index
    j = node index
    N = number of nodes
    M = N-1 = number of segements
    O--------O--------O----...----O--------O--------O
    j=0      j=1      j=2         j=N-2    j=N-1    j=N
  */

  // a priori
  const int nseg;
  const Spacecraft spacecraft;
  std::vector<Body> bodies;

  // a posteriori
  double t0;
  double tN;
  Spacecraft::State state0;
  Spacecraft::State stateN;
  std::vector<Spacecraft::Control> controls;
  std::vector<Spacecraft::State> states;

  // constructor
  Phase (
    const int & nseg_,
    const Spacecraft spacecraft_
  ) : nseg(nseg_), spacecraft(spacecraft_) {
    controls.resize(nseg);
  };

  // destructor
  ~Phase (void) {};

  // set initial time
  void set_initial_time (const double & t0_) {t0 = t0_;};

  // set final time
  void set_final_time (const double & tN_) {tN = tN_;};

  // set both times
  void set_times (const double & t0_, const double & tN_) {
    set_initial_time(t0_);
    set_final_time(tN_);
  };

  // set intial state
  void set_initial_state (const Spacecraft::State & state0_) {
    state0 = state0_;
  };

  // set intial state
  void set_final_state (const Spacecraft::State & stateN_) {
    stateN = stateN_;
  };

  // set both states
  void set_states (
    const Spacecraft::State & state0_,
    const Spacecraft::State & stateN_
  ) {
    set_initial_state(state0_);
    set_final_state(stateN_);
  };

  // set states and times
  void set_phase (
    const Spacecraft::State & state0_,
    const Spacecraft::State & stateN_,
    const double & t0_,
    const double & tN_
  ) {
    set_states(state0_, stateN_);
    set_times(t0_, tN_);
  };

  // set random controls
  void set_random_controls (void) {
    for (int seg=0; seg<nseg; seg++) {
      controls[seg] = Spacecraft::Control::Random().normalized();
    };
  };

  // set controls with throttle vector
  void set_throttles (const std::vector<double> & throttles_) {
    if (throttles_.size() != nseg*3) {
      throw std::invalid_argument("Throttle vector must match.");
    };
    for (int i=0, seg=0; i<throttles_.size() && seg<nseg; i=i+3, seg++) {
      for (int dim=0; dim<3; dim++) {
        controls[seg][dim] = throttles_[i+dim];
      };
    };
  };

  // add gravitational influences
  void add_body (const Body & body_) {
    bodies.push_back(body_);
  };

  // numerical integration
  void propagate(
    Spacecraft::State & x,
    const Spacecraft::Control & u,
    double t0,
    double t1,
    double dt
  ) {
    ODE sys(u, *this);
    std::vector<double> _x(x.data(), x.data() + x.rows() * x.cols());
    std::vector<ODE::state_type> states;
    std::vector<double> times;
    boost::numeric::odeint::integrate(sys, _x, t0, t1, dt, ODE_record(states, times));
    std::cout << "Integration\n";
    for (int i=0; i<states.size(); i++) {
      for (int j=0; j<7; j++) {
        std::cout << states[i][j] << "\t";
      };
      std::cout << "\n";
    };

  };


  private:

    // odeint workaround
    struct ODE {

      // definitions
      typedef std::vector<double> state_type;

      // members
      const Spacecraft::Control & F;
      Phase & phase;

      // constructor
      ODE (
        const Spacecraft::Control & u_,
        Phase & phase_
      ) : F(u_*phase_.spacecraft.thrust), phase(phase_) {};

      // destructor
      ~ODE (void) {};

      // reimplemented motion
      void operator () (
        const state_type & x_,
        state_type & dxdt_,
        const double t
      ) {
        // first order ode system
        // NOTE: Eigen routines are not called by odeint
        //Spacecraft::Control F = u*phase.spacecraft.thrust;
        dxdt_[0] = x_[3]; // vx
        dxdt_[1] = x_[4]; // vy
        dxdt_[2] = x_[5]; // vz
        dxdt_[3] = F(0)/x_[6]; // propulsion x
        dxdt_[4] = F(1)/x_[6]; // propulsion y
        dxdt_[5] = F(2)/x_[6]; // propulsion z
        dxdt_[6] = -F.norm()/(phase.spacecraft.g0*phase.spacecraft.isp); // mdot
        // assemble gravitational influences
        Eigen::Vector3d r;
        double r3;
        for (int i=0; i<phase.bodies.size(); i++) {
          r = phase.bodies[i].eph(t).block(0,0,3,1);
          r(0) = x_[0] - r(0);
          r(1) = x_[1] - r(1);
          r(2) = x_[2] - r(2);
          r3 = pow(r.norm(), 3);
          dxdt_[3] += -phase.bodies[i].mu*r(0)/r3;
          dxdt_[4] += -phase.bodies[i].mu*r(1)/r3;
          dxdt_[5] += -phase.bodies[i].mu*r(2)/r3;
        };
      };

    };

    struct ODE_record {

      // members
      std::vector<ODE::state_type> & states;
      std::vector<double> & times;

      // constructor
      ODE_record (
        std::vector<ODE::state_type> & states_,
        std::vector<double> & times_
      ) : states(states_), times(times_) {};

      // destructor
      ~ODE_record(void) {};

      void operator () (const ODE::state_type & x, double t) {
        states.push_back(x);
        times.push_back(t);
      };

    };


};
#endif
