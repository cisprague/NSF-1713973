// Christopher Iliffe Sprague
// https://cisprague.github.io
// cisprague@ac.jaxa.jp

#ifndef phs_hpp
#define phs_hpp

#include <iostream>
#include "sc.hpp"
#include "pnt.hpp"

class Phase {

  public:

    // members
    Point::State state0; // initial state
    Point::State state1; // final s tate
    double t0;           // initial time
    double t1;           // final time
    int    nn;           // collocation nodes

    // constructor
    Phase(int nn_) {
      nn = nn_;
      cout << "Leg " << this << " constructed \n" << endl;
    };

    // destructor
    ~Phase(void) {
      cout << "Leg " << this << " destroyed.\n" << endl;
    };

    // modify
    void set(Point::State s0_, Point::State s1_, double t0_, double t1_) {
      state0 = s0_;
      state1 = s1_;
      t0     = t0_;
      t1     = t1_;
    };


};

#endif
