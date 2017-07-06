// Christopher Iliffe Sprague
// https://cisprague.github.io
// cisprague@ac.jaxa.jp

#ifndef plnt_hpp
#define plnt_hpp

#include <string>
#include <vector>
#include <iostream>
#include <eigen3/Eigen/Dense>
#include "SpiceUsr.h"
#include "matplotlibcpp.h"

using namespace std;
using namespace Eigen;

// planet obviously
class plnt {

  public:

    // usefull types
    typedef Matrix<double, 6, 1> State;
    typedef vector<State> States;

    // constants
    string name;      // name obviously                  [text]
    int id;           // spice ID                        [ID]
    double radius;    // terrestrial radius              [m]
    double mu;        // gravitational parametre         [m^3/s^2]
    State state;      // position and velocity           [m, m/s]

    // constructor
    plnt(string name_) {
      name = name_;

      // find spice id
      SpiceBoolean found;
      bodn2c_c(name.c_str(), &id, &found);

      // find radii
      SpiceInt dim;
      SpiceDouble radii[3];
      bodvcd_c(id, "RADII", 3, &dim, radii);
      radius = (radii[0] + radii[1] + radii[2])/3;
      radius = radius*1000;

      // find gravitational parametre
      bodvcd_c(id, "GM", 1, &dim, &mu);
      mu = mu*pow(1000,3); // convert to m^3/s^2

      // message
      cout << "Planet " << name << " (" << this << ") ";
      cout << "constructed." << endl;
    };

    // destructor
    ~plnt(void) {
      cout << "Planet " << name << " (" << this << ") ";
      cout << "destructed." << endl;
    };

    // print details
    void print(void) {
      cout << "Planet " << name << " (" << this << ") ";
      cout << "initialised with constants:\n";
      cout << "R = " << radius << " [m]\t";
      cout << "Mu = " << mu << " [m^3/s^2]" << endl;
    };

    // compute state
    void eph(double epoch, string obs) {
      SpiceDouble lt;
      SpiceDouble st[6];
      spkezr_c(name.c_str(), epoch, "J2000", "NONE", obs.c_str(), st, &lt);
      for (int i=0; i<6; i++) {state(i) = st[i]*1000;};
    };


};

// jpl spice
namespace spc {

  // load kernel
  void ld_krnl(string fname) {
    furnsh_c(fname.c_str());
  };

  // compute acceleration
  

};

#endif
