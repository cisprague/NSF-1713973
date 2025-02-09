#ifndef spice_hpp
#define spice_hpp
#include <string>
#include <vector>
#include "SpiceUsr.h"

namespace spice {

  void load_kernels (void) {
    trcoff_c();
    furnsh_c("../src/req/cspice/kernels/DE.bsp");
    furnsh_c("../src/req/cspice/kernels/GM.tpc");
    furnsh_c("../src/req/cspice/kernels/L2.bsp");
    furnsh_c("../src/req/cspice/kernels/L4.bsp");
    furnsh_c("../src/req/cspice/kernels/LS.tls");
    furnsh_c("../src/req/cspice/kernels/ORMS.bsp");
    furnsh_c("../src/req/cspice/kernels/PCK.tpc");
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
