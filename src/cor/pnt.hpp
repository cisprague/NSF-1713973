// Christopher Iliffe Sprague
// https://cisprague.github.io
// cisprague@ac.jaxa.jp

#ifndef pnt_hpp
#define pnt_hpp

#include <string>
#include <vector>
#include <iostream>
#include <eigen3/Eigen/Dense>
#include "SpiceUsr.h"
//#include "matplotlibcpp.h"

using namespace std;
using namespace Eigen;

// point in space
class Point {

  public:

    // usefull types
    typedef Matrix<double, 6, 1> State;
    typedef vector<State>        States;

    // constants
    string name;   // name obviously [text]
    int    id;     // jpl spice id
    State  state;  // position and velocity
    States states; // state history

    // constructor
    Point(string name_) {
      name = name_;
      SpiceBoolean found;
      bodn2c_c(name.c_str(), &id, &found); // find spice id
      cout << "Created point " << name;
      cout << " (" << this << ") with SPICE ID " << id << "\n" << endl;
    };

    // destructor
    ~Point(void) {
      cout << "Destroyed " << name << this << "\n" << endl;
    };

    // compute state
    void eph(double epoch, string obs) {
      SpiceDouble lt;
      SpiceDouble st[6];
      spkezr_c(name.c_str(), epoch, "J2000", "NONE", obs.c_str(), st, &lt);
      for (int i=0; i<6; i++) {state(i) = st[i]*1000;};
    };


};

class Body : public Point {

  public:

    // constants
    double mu;     // graviational parametre
    double radius; // radii

    // constructor
    Body(string name_) : Point(name_) {

      // find graviational parametre
      SpiceInt dim;
      bodvcd_c(id, "GM", 1, &dim, &mu);
      mu = mu*pow(1000,3); // convert to m^3/s^2

      // find radius
      SpiceDouble radii[3];
      bodvcd_c(id, "RADII", 3, &dim, radii);
      radius = (radii[0] + radii[1] + radii[2])/3;
      radius = radius*1000;
      cout << "Created body " << name;
      cout << " (" << this << ") with radius " << radius << "\n" << endl;

    };

};

// jpl spice
namespace spc {

  // load kernel
  void ld_krnl(string fname) {
    furnsh_c(fname.c_str());
  };

};


#endif
