// Christopher Iliffe Sprague
// https://cisprague.github.io
// cisprague@ac.jaxa.jp

#ifndef spc_hpp
#define spc_hpp

#include <vector>
#include <iostream>
#include <eigen3/Eigen/Dense>

using namespace std;
using namespace Eigen;

// fundamental spacecraft
class Spacecraft {

  public:

    // usefull types
    typedef Matrix<double, 7, 1> State;    // s_i = {x,y,z,vx,vy,vz,m}_i
    typedef vector<State>        States;   // S   = {s_i,...,s_N}
    typedef Matrix<double, 3, 1> Control;  // u_i = {ux,uy,uz}_i
    typedef vector<Control>      Controls; // U   = {u_i,...,u_M}

    // constants
    double mass;   // propellant       [kg]
    double thrust; // max thrust       [N]
    double isp;    // specific impulse [s]

    // cartesian element histories
    States   states;
    Controls controls;

    // constructor
    Spacecraft(double mass_, double thrust_, double isp_) {
      mass   = mass_;
      thrust = thrust_;
      isp    = isp_;
      cout << "Spacecraft "   << this   << " initialised with constants:\n";
      cout << "Mass = "       << mass   << " [kg]\t";
      cout << "Max Thrust = " << thrust << " [N]\t";
      cout << "Max Isp = "    << isp    << " [s]\n" << endl;
    };

    // destructor
    ~Spacecraft(void) {
      cout << "Spacecraft " << this << " desctructed\n" << endl;
    };

    // print details
    void print(void) {
      cout << "Spacecraft "   << this   << ":\n";
      cout << "Mass = "       << mass   << " [kg]\t";
      cout << "Max Thrust = " << thrust << " [N]\t";
      cout << "Max Isp = "    << isp    << " [s]\n" << endl;
    };

    // set constants
    void set_constants(double mass_, double thrust_, double isp_) {
      mass   = mass_;
      thrust = thrust_;
      isp    = isp_;
      cout << "Spacecraft "   << this   << " new constants:\n";
      cout << "Mass = "       << mass   << " [kg]\t";
      cout << "Max Thrust = " << thrust << " [N]\t";
      cout << "Max Isp = "    << isp    << " [s]" << endl;
    };

    // set state
    void set_initial_state(State state0_) {
      states.clear();            // reset the trajectory
      states.push_back(state0_); // new initial state
      cout << "Spacecraft " << this << " new state:\n";
      cout << "[" << states[0].transpose() << "]" << endl;
    };

};

#endif
