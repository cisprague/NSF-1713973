// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp
// NSF Award 1713973

#ifndef body_hpp
#define body_hpp
#include <string>
#include "spice.hpp"
#include "linalg.hpp"
#include "matplotlibcpp.h"

struct Body {

  // constants
  const std::string name;
  const double      radius;
  const double      mu;
  const int         id;

  // constructor
  Body     (const std::string & name_) :
    name   (name_                    ),
    radius (spice::radius (name_)    ),
    mu     (spice::mu     (name_)    ),
    id     (spice::id     (name_)    ) {};

  // destructor
  ~Body (void) {};

  // ephemerides
  std::vector<double> state (
    const double & mjd2000,
    const std::string & obs = "SSB"
  ) const {
    return spice::state(mjd2000, name, obs);
  };

  std::vector<std::vector<double>> states (
    const std::vector<double> & times,
    const std::string & obs = "SSB"
  ) const {
    // states
    std::vector<std::vector<double>> slist(6);
    // number of points
    const int npts(times.size());
    // for each point in time
    for (int i=0; i<npts; ++i) {
      // compute the state of the body
      const std::vector<double> s(state(times.at(i), obs));
      // for each dimension
      for (int j=0; j<6; ++j) {
        slist.at(j).push_back(s.at(j));
      };
    };
    return slist;
  };

  // plotting
  void plot (
    const int & xi,
    const int & yi,
    const std::vector<double> & times,
    const std::string & obs = "SSB",
    const std::string & mark = "k,"
  ) const {
    // compute trajectory of body
    const std::vector<std::vector<double>> slist(states(times, obs));
    // plot the dimensions of interest
    matplotlibcpp::plot(slist.at(xi), slist.at(yi), mark);
  };

  // plotting with multiple time sequences
  void plot (
    const int & xi,
    const int & yi,
    const std::vector<std::vector<double>> & times,
    const std::string & obs = "SSB",
    const std::string & mark = "k,"
  ) const {
    // how many time sequences we have
    const int nseq(times.size());
    // for each sequence
    for (int i=0; i<nseq; ++i) {
      // plot for this sequence
      plot(xi, yi, times.at(i), obs, mark);
    };
  };

  // position
  std::vector<double> position (
    const double & mjd2000,
    const std::string & obs = "SSB"
  ) const {
    return linalg::section(state(mjd2000), 0, 2);
  };

  // velocity
  std::vector<double> velocity (
    const double & mjd2000,
    const std::string & obs = "SSB"
  ) const {
    return linalg::section(state(mjd2000), 3, 5);
  };

};

#endif
