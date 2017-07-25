// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp
// NSF Award 1713973

#ifndef body_hpp
#define body_hpp
#include <string>
#include "spice.hpp"
#include "orbits.hpp"
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

  // state wrt state
  std::vector<double> state_wrt (
    const double & mjd2000,
    const std::vector<double> & sbary
  ) const {
    return linalg::diff(sbary, state(mjd2000));
  };

  // random orbit
  std::vector<double> random_orbit (
    const double & mjd2000,
    const double & hl,
    const double & hu,
    const std::string & obs = "SSB"
  ) const {
    // compute state of this planet at time
    std::vector<double> s = state(mjd2000, obs);
    // compute random orbit wrt obs
    return orbit::random_obs_orbit(s, mu, hl+radius, hu+radius);
  };
};

#endif
