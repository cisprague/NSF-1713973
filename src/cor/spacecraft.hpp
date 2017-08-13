#ifndef spacecraft_hpp
#define spacecraft_hpp
#include <vector>
#include <cmath>
#include "constants.hpp"

struct Spacecraft {

  // constants
  const double mass;
  const double thrust;
  const double isp;
  const double veff;

  // constructor
  Spacecraft (
    const double & mass_,
    const double & thrust_,
    const double & isp_
  ) : mass(mass_), thrust(thrust_), isp(isp_), veff(G_0*isp) {};

  // destructor
  ~Spacecraft (void) {};

  // control vector validity
  void check_control (const std::vector<double> & u) const {
    // dimension of control
    const int dim(u.size());
    // check dimension
    if (dim != 3) {throw "Control vector must be 3D";};
  };

  // force vector
  std::vector<double> force (const std::vector<double> & u) const {
    check_control(u);
    std::vector<double> f(u);
    for (int i=0; i<3; ++i) {f[i] *= thrust;};
    return f;
  };

  // force magnitude
  double fmag (const std::vector<double> & u) const {
    const std::vector<double> f(force(u));
    double fmag(0);
    for (int i=0; i<3; ++i) {fmag += pow(f[i], 2);};
    fmag = sqrt(fmag);
    return fmag;
  };

  // mass flow rate
  double mdot (const std::vector<double> & u) const {
    return -fmag(u)/veff;
  };


};

#endif
