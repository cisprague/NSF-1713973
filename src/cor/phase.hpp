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
  std::vector<double> times;

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
    const double & t0,
    const double & t1,
    const double & dt
  ) const {

    // set up ODE with control and reference to intrinsics
    ODE ode(u, *this);

    // set up ODE recorder
    //ODE_record ode_record();

    // convert eigen vector to std::vector...
    ODE::state_type xode;
    xode.resize(7);
    for (int i=0; i<7; i++) {xode[i] = x(i);};
    ODE::state_type dxdt;

    ode(xode, dxdt, t0);

    // numerically integrate
    //boost::numeric::odeint::integrate(ode, xode, t0, t1, dt, display);
  };


  private:

    // odeint workaround...
    struct ODE {

      // definitions
      typedef std::vector<double> state_type; // for odeint...

      // members
      const Spacecraft::Control & F;     // propulsion force vector
      const double              & veff;  // effective velocity
      const double              & Fnorm; // propulsion force magnitude
      const Phase               & phase; // access to phase intrinsics

      // constructor
      ODE (const Spacecraft::Control & u_, const Phase & phase_) :
        phase(phase_),
        F(u_*phase_.spacecraft.thrust),
        veff(phase_.spacecraft.isp*phase_.spacecraft.g0),
        Fnorm(F.norm()) {};

      // destructor
      ~ODE (void) {};

      // dynamics
      void operator () (
        const state_type & x,
        state_type & dxdt,
        const double t
      ) const {

        // propulsion influences
        dxdt[0] = x[3];        // vx
        dxdt[1] = x[4];        // vy
        dxdt[2] = x[5];        // vz
        dxdt[3] = F(0)/x[6];   // aux
        dxdt[4] = F(1)/x[6];   // auy
        dxdt[5] = F(2)/x[6];   // auz
        dxdt[6] = -Fnorm/veff; // mdot

        // gravitational influences
        Eigen::Matrix<double,3,1> r;
        double r3;
        for (int i=0; i<phase.bodies.size(); i++) {
          r = phase.bodies[i].eph(t).block(0,0,3,1);
          r(0) = x[0] - r(0);
          r(1) = x[1] - r(1);
          r(2) = x[2] - r(2);
          r3  = pow(r.norm(), 3);
          dxdt[3] -= phase.bodies[i].mu*r(0)/r3;
          dxdt[4] -= phase.bodies[i].mu*r(1)/r3;
          dxdt[5] -= phase.bodies[i].mu*r(2)/r3;

        };

      };

    };

    static void display(const ODE::state_type & x, const double t) {
      std::cout << t << '\t';
      for (int i=0; i<7; i++) {
        std::cout << x[i] << '\t';
      };
      std::cout << '\n' << std::endl;
    };

};
#endif
