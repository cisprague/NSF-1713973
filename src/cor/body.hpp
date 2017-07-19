#ifndef body_hpp
#define body_hpp
#include <string>
#include <eigen3/Eigen/Dense>
#include "SpiceUsr.h"

struct Body {

  // definitions
  typedef Eigen::Matrix <double, 6, 1> State;

  // constants
  std::string name;
  double radius;
  double mu;
  int id;

  // constructor
  Body (std::string name_) {
    name = name_;
    // spice id
    SpiceBoolean found;
    bodn2c_c(name.c_str(), &id, &found);
    // graviational parametre
    SpiceInt dim;
    bodvcd_c(id, "GM", 1, &dim, &mu);
    mu = mu*pow(1000,3);
    // radius
    SpiceDouble radii[3];
    bodvcd_c(id, "RADII", 3, &dim, radii);
    radius = 1000*(radii[0] + radii[1] + radii[2])/3;
  };

  // destructor
  ~Body (void) {};

  // ephemerides
  State eph (const double mjd2000, const std::string obs = "SSB") {
    SpiceDouble lt;
    SpiceDouble st[6];
    spkezr_c(name.c_str(), mjd2000, "J2000", "NONE", obs.c_str(), st, &lt);
    State state;
    for (int i=0; i<6; i++) {state(i) = st[i]*1000;};
    return state;
  };

};

void load_kernels (void) {
  furnsh_c("../dta/de430.bsp");
  furnsh_c("../dta/gm_de431.tpc");
  furnsh_c("../dta/naif0011.tls");
  furnsh_c("../dta/pck00008.tpc");
};

#endif
