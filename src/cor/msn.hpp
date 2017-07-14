// Christopher Iliffe Sprague
// cisprague@ax.jaxa.jp

#ifndef msn_hpp
#define msn_hpp

#include <pagmo/problem.hpp>
#include <pagmo/types.hpp>
#include "../cor/fika.hpp"

using namespace pagmo;

namespace Mission {

  // point to point
  // z = {t0,tN,ux0,uy0,uz0,...,uxM,uyM,uzM} : M = N-1 : N nodes, M segements
  struct PTP {

    // a priori members
    Spacecraft & sc;   // spacecraft
    Point      & p0;   // origin
    Point      & p1;   // destination

    // a posteriori members
    Phase      phase;  // phase
    int        nnodes; // collocation nodes

    // constructor
    PTP(Spacecraft &sc_, Point &p0_, Point &p1_, int nn): sc(sc_), p0(p0_), p1(p1_) {
      cout << this << " problem constructed. \n" << endl;
    };

    // desctructor
    ~PTP(void) {
      cout << this << " problem desctructed. \n" << endl;
    };

    // objective, equalities, inequalities
    vector_double fitness(const vector_double & z) const {
      return {z[0], z[1], z[2]};
    };

  };

};

#endif
