#ifndef body_hpp
#define body_hpp
#include <string>
#include <eigen3/Eigen/Dense>
#include <random>
#include <cmath>
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
  State eph (const double mjd2000, const std::string obs = "SSB") const {
    SpiceDouble lt;
    SpiceDouble st[6];
    spkezr_c(name.c_str(), mjd2000, "J2000", "NONE", obs.c_str(), st, &lt);
    State state;
    for (int i=0; i<6; i++) {state(i) = st[i]*1000;};
    return state;
  };

};

// random circular orbit about body at time
Body::State random_orbit(
  const Body & body,
  const double & t,
  const double & hl = 160e3,
  const double & hu = 2000e3
) {

  // random things
  std::random_device rd;  // for seed
  std::mt19937 gen(rd()); // mersenne twister engine
  std::uniform_real_distribution<double> u(0, 1);    // for latitude
  std::uniform_real_distribution<double> w(0, 1);    // for longitude
  std::uniform_real_distribution<double> h(hl, hu);  // altitude
  std::uniform_real_distribution<double> yarb(-1,1); // arbitrary y
  std::uniform_real_distribution<double> zarb(-1,1); // arbitrary z

  // orbit specifications
  double theta = 2*M_PI*u(gen);        // latitude
  double phi   = acos(2*w(gen) - 1);   // longitude
  double rmag  = h(gen) + body.radius; // geocentric distance

  // geocentric position
  Eigen::Matrix <double,3,1> r;
  r(0) = rmag*sin(phi)*cos(theta); // x
  r(1) = rmag*sin(phi)*sin(theta); // y
  r(2) = rmag*cos(phi);            // z

  // arbitrary point
  Eigen::Matrix <double, 3, 1> pa;
  pa(1) = yarb(gen);                                                // ya
  pa(2) = zarb(gen);                                                // za
  pa(0) = r(0) + (-r(1)*(pa(1) - r(1)) - r(2)*(pa(2) - r(2)))/r(0); // xa

  // geocentric velocity direction
  Eigen::Matrix <double, 3, 1> vhat;
  vhat = pa - r;
  vhat = vhat.normalized();

  // geocentric velocity
  Eigen::Matrix <double, 3, 1> v;
  v = sqrt(body.mu/r.norm())*vhat;

  // geocentric state
  Body::State s;
  s << r, v;

  // barycentric state
  s += body.eph(t);

  return s;
};

void load_kernels (void) {
  furnsh_c("../dta/de430.bsp");
  furnsh_c("../dta/gm_de431.tpc");
  furnsh_c("../dta/naif0011.tls");
  furnsh_c("../dta/pck00008.tpc");
};

#endif
