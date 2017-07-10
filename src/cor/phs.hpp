// Christopher Iliffe Sprague
// https://cisprague.github.io
// cisprague@ac.jaxa.jp

#ifndef phs_hpp
#define phs_hpp

#include <iostream>
#include "spc.hpp"

class Phase {

  public:

    // members
    Spacecraft & sc;          // spacecraft undertaking leg
    Spacecraft::State state0; // initial state
    Spacecraft::State state1; // final state
    double t0;                // initial time
    double t1;                // final time

    // constructor
    Phase(
      Spacecraft & sc_,
      Spacecraft::State s0_,
      Spacecraft::State s1_,
      double t0,
      double t1
    ) : sc(sc_) {
      state0 = s0_;
      state1 = s1_;
      cout << "Leg " << this << " initialised with Spacecraft " << &sc_ << endl;
      cout << "Initial state: " << state0.transpose() << endl;
      cout << "Final state: " << state1.transpose() << "\n" << endl;
    };

    // destructor
    ~Phase(void) {
      cout << "Leg " << this << " destroyed.\n" << endl;
    };

};

#endif
