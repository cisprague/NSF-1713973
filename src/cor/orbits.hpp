// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp
// NSF Award 1713973

#ifndef orbits_hpp
#define orbits_hpp
#include <vector>
#include <random>
#include <cmath>
#include "linalg.hpp"

namespace orbit {

  std::vector<double> random_geo_orbit (
    const double & mu,
    const double & dl,
    const double & du
  ) {

    // random things
    std::random_device rd;                             // seeding
    std::mt19937 gen(rd());                            // mersenne twister
    std::uniform_real_distribution<double> u(0, 1);    // for latitude
    std::uniform_real_distribution<double> w(0, 1);    // for longitude
    std::uniform_real_distribution<double> d(dl, du);  // distance
    std::uniform_real_distribution<double> yarb(-1,1); // arbitrary y
    std::uniform_real_distribution<double> zarb(-1,1); // arbitrary z

    // orbit specifications
    double theta = 2*M_PI*u(gen);      // latitude
    double phi   = acos(2*w(gen) - 1); // longitude
    double rmag  = d(gen);             // geocentric distance

    // geocentric position
    std::vector<double> r(3);
    r[0] = rmag*sin(phi)*cos(theta); // x
    r[1] = rmag*sin(phi)*sin(theta); // y
    r[2] = rmag*cos(phi);            // z

    linalg::display_vec(r);

    // arbitrary point
    std::vector<double> pa(3);
    pa[1] = yarb(gen);                                            // ya
    pa[2] = zarb(gen);                                            // za
    pa[0] = r[0] + (-r[1]*(pa[1]-r[1]) - r[2]*(pa[2]-r[2]))/r[0]; // xa

    // geocentric velocity direction
    std::vector<double> vhat(3);
    for (int i=0; i<3; i++) {vhat[i] = pa[i] - r[i];};
    double vhatnorm = 0;
    for (int i=0; i<3; i++) {vhatnorm += pow(vhat[i], 2);}; // squared sum
    vhatnorm = sqrt(vhatnorm);                              // Euclidan norm
    for (int i=0; i<3; i++) {vhat[i] /= vhatnorm;};         // normalise

    // geocentric velocity
    std::vector<double> v(3);
    double vmag = sqrt(mu/rmag);
    for (int i=0; i<3; i++) {v[i] = vmag*vhat[i];};

    // geocentric state
    std::vector<double> s(6);
    for (int i=0; i<3; i++) {s[i] = r[i];};
    for (int i=3; i<6; i++) {s[i] = v[i];};

    return s;
  };

  std::vector<double> random_obs_orbit (
    const std::vector<double> & sobs,
    const double & mu,
    const double & dl,
    const double & du
  ) {

    // geocentric state
    std::vector<double> s = random_geo_orbit(mu, dl, du);

    // compute state wrt observer
    for (int i=0; i<6; i++) {s[i] += sobs[i];};

    return s;
  };

};



#endif
