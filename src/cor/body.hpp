// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp
// NSF Award 1713973

#ifndef body_hpp
#define body_hpp
#include <string>
#include "spice.hpp"
#include "linalg.hpp"

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
