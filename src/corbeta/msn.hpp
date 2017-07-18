// Christopher Iliffe Sprague
// cisprague@ax.jaxa.jp

#ifndef msn_hpp
#define msn_hpp

#include <iostream>
#include <string>

#include "../cor/sc.hpp"
#include "../cor/pnt.hpp"
#include "../cor/phs.hpp"

using namespace std;

namespace Mission {

  // point to point
  // zi = {t0,tN,ux0,uy0,uz0,...,uxM,uyM,uzM}i : M = N-1 : N nodes, M segements
  struct PTP {

    // a priori members
    Spacecraft & sc;
    Point      & p1;
    Point      & p2;

    // a posteriori members
    //Phase traj;

    // constructor
    PTP(Spacecraft sc_, Point p1_, Point p2_) : sc(sc_), p1(p1_), p2(p2_) {
      cout << this << " mission constructed.\n" << endl;
    };

    ~PTP(void) {
      cout << this << " mission destroyed.\n" << endl;
    };

  };

};

#endif
