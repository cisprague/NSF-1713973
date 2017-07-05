// Christopher Iliffe Sprague
// https://cisprague.github.io
// cisprague@ac.jaxa.jp

#ifndef utl_hpp
#define utl_hpp

#include <string>
#include <cmath>
#include "../req/cspice/include/SpiceUsr.h"

using namespace std;

// jpl spice
namespace spc {

  // load kernel
  void ld_krnl(string fname) {
    furnsh_c(fname.c_str());
  };

  // body info
  void plnt_info(string &name, int &id, double &radius, double &mu) {

    // find spice id
    SpiceBoolean found;
    bodn2c_c(name.c_str(), &id, &found);

    // find radius
    SpiceInt dim;
    SpiceDouble radii[3];
    bodvcd_c(id, "RADII", 3, &dim, radii);
    radius = (radii[0] + radii[1] + radii[2])/3;
    radius = radius*1000; // convert to m

    // find gravitational parametre
    bodvcd_c(id, "GM", 1, &dim, &mu);
    mu = mu*pow(1000,3); // convert to m^3/m^2

  };

};

#endif
