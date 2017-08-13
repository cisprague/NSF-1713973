#ifndef body_hpp
#define body_hpp
#include <string>
#include <vector>
#include "spice.hpp"

struct Body {

  // constants
  const std::string name;
  const double radius;
  const double mu;
  const int id;

  // constructor
  Body (const std::string & name_) :
    name(name_),
    radius(spice::radius(name_)),
    mu(spice::mu(name_)),
    id(spice::id(name_)) {};

  // destructor
  ~Body (void) {};

  // state via ephemeris
  std::vector<double> state (
    const double & mjd2000,
    const std::string & obs = "SSB"
  ) const {
    return spice::state(mjd2000, name, obs);
  };

  // trajectory of states
  std::vector<std::vector<double>> states (
    const std::vector<double> & times,
    const std::string & obs = "SSB"
  ) const {
    // vector of individual state trajectories
    std::vector<std::vector<double>> slist(6);
    // number of times
    const int npts(times.size());
    // for each time
    for (int i=0; i<npts; ++i) {
      const std::vector<double> s(state(times[i], obs));
      // for each dimension
      for (int j=0; j<6; ++j) {
        slist[j].push_back(s[j]);
      };
    };
    return slist;
  };

};

#endif
