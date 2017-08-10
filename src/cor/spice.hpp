// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp
// NSF Award 1713973

#ifndef spice_hpp
#define spice_hpp
#include <string>
#include <vector>
#include "SpiceUsr.h"

namespace spice {

  void load_kernels (void) {
    furnsh_c("../dta/spice/de430.bsp");
    furnsh_c("../dta/spice/L2_de431.bsp");
    furnsh_c("../dta/spice/de421.bsp");
    furnsh_c("../dta/spice/gm_de431.tpc");
    furnsh_c("../dta/spice/naif0011.tls");
    furnsh_c("../dta/spice/pck00008.tpc");
    furnsh_c("../dta/spice/pck00008.tpc");
    furnsh_c("../dta/spice/ORMS__041111020517_00206.BSP");
  };

  int id (const std::string & name) {
    int id;
    int found;
    bodn2c_c(name.c_str(), &id, &found);
    return id;
  };

  double mu (const std::string & name) {
    int dim;
    double mu;
    bodvcd_c(id(name), "GM", 1, &dim, &mu);
    mu *= 1000*1000*1000;
    return mu;
  };

  double radius (const std::string & name) {
    int dim;
    double radii[3];
    double radius;
    bodvcd_c(id(name), "RADII", 3, &dim, radii);
    radius = 1000*(radii[0] + radii[1] + radii[2])/3;
    return radius;
  };

  std::vector<double> state (
    const double & mjd2000,
    const std::string & targ,
    const std::string & obs = "SSB"
  ) {
    double lt;
    double st[6];
    spkezr_c(targ.c_str(), mjd2000, "J2000", "NONE", obs.c_str(), st, &lt);
    std::vector<double> state(st, st + sizeof st / sizeof st[0]);
    for (int i=0; i<6; i++) {state[i] *= 1000;};
    return state;
  };

  double mjd2000 (const std::string & date) {
    double mjd2000;
    str2et_c(date.c_str(), &mjd2000);
    return mjd2000;
  };

};


#endif
