// Christopher Iliffe Sprague
// https://cisprague.github.io
// cisprague@ac.jaxa.jp

#ifndef spc_hpp
#define spc_hpp

#include <string>
#include "../req/cspice/include/SpiceUsr.h"
#include "../req/cspice/include/SpiceZfc.h"
#include "../req/cspice/include/SpiceZmc.h"

using namespace std;

// JPL SPICE
namespace spc {

  // load kernel
  void ld_krnl(string fname) {
    furnsh_c(fname.c_str());
  };

  // state of body



};




#endif
