// Christopher Iliffe Sprague
// https://cisprague.github.io
// cisprague@ac.jaxa.jp

#ifndef plnt_hpp
#define plnt_hpp

#include <string>
#include <vector>
#include <iostream>
#include <eigen3/Eigen/Dense>
#include <vector>

using namespace std;
using namespace Eigen;

// planets obviously
namespace plnt {

  // base
  class base {

    public:

      // usefull types
      typedef Matrix<double, 6, 1> State;
      typedef vector<State> States;

      // constants
      string name;   // name obviously                  [text]
      double radius; // terrestrial radius              [m]
      double soi;    // sphere of influence             [m]
      double mu;     // gravitational parametre         [m^3/s^2]
      double mup;    // primary gravitational parametre [m^3/s^2]
      States state;  // position and velocity           [m, m/s]


      // constructor
      base(string name_, double radius_, double soi_, double mu_, double mup_) {
        name   = name_;
        radius = radius_;
        soi    = soi_;
        mu     = mu_;
        mup    = mup_;
        cout << "Planet " << name << " (" << this << ") ";
        cout << "initialised with constants:\n";
        cout << "Radius = " << radius << " [m]\t";
        cout << "Sphere of influence = " << " [m]\t";
        cout << "Gravitational parametre = " << " [m^3/s^2]\t";
        cout << "Primary gravitational parametre = " << " [m^3/m^2]" << endl;
      };

      // destructor
      virtual ~base(void)

  };

  // spicy planet
  class spc : public base {

    public:

      // constants
      string ref; // reference frame

      // constructor
      spc(
        string name_,
        double radius_,
        double soi_,
        double mu_,
        double mup_,
        string ref_
      ) {

      };






  };

};

#endif
