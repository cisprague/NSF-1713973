#ifndef spacecraft_hpp
#define spacecraft_hpp
#include <eigen3/Eigen/Dense>

struct Spacecraft {

  // definitions
  typedef Eigen::Matrix <double, 7, 1> State;
  typedef Eigen::Matrix <double, 3, 1> Control;

  // constants
  const double mass;
  const double thrust;
  const double isp;
  const double g0 = 9.80665;

  // constructor
  Spacecraft (
    const double & mass_,
    const double & thrust_,
    const double & isp_
  ) : mass(mass_), thrust(thrust_), isp(isp_) {};

  // destructor
  ~Spacecraft (void) {};

};

#endif
