// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp

#ifndef ptp_hpp
#define ptp_hpp
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <tuple>
#include "yaml.h"
#include "pagmo.hpp"

#include "../cor/spacecraft.hpp"
#include "../cor/spice.hpp"
#include "../cor/body.hpp"
#include "../cor/controller.hpp"
#include "../cor/phase.hpp"



struct PTP {

  // definition
  typedef std::tuple<double, double, double, std::vector<double>, std::vector<double>> Decision;

  // members
  Phase<Controller::Relative> phase;

  // constructor
  PTP (const std::string & fpath) : phase(init(fpath)) {};

  // destructor
  ~PTP (void) {};

  // get current decision
  std::vector<double> get_decision (void) {

    // create decision vector
    std::vector<double> dv;

    // boundary conditions
    dv.push_back(phase.t0);    // initial time
    dv.push_back(phase.tN);    // final time
    dv.push_back(phase.xN[6]); // final mass

    // network parametres
    std::vector<double> weights(phase.controller.mlp.get_weights());
    std::vector<double> biases(phase.controller.mlp.get_biases());

    // append weights
    for (int i=0; i<phase.controller.mlp.wd; ++i) {
      dv.push_back(weights[i]);
    };

    // append biases
    for (int i=0; i<phase.controller.mlp.bd; ++i) {
      dv.push_back(biases[i]);
    };

    return dv;
  };

  // initialiser
  static Phase<Controller::Relative> init (const std::string & fpath) {

    // problem configuration. TODO: fix to use relative path
    YAML::Node config(YAML::LoadFile(fpath));

    // spacecraft
    double mass  (config["mass"  ].as<double>());
    double thrust(config["thrust"].as<double>());
    double isp   (config["isp"   ].as<double>());
    Spacecraft sc(mass, thrust, isp);

    // load ephemerides
    spice::load_kernels();

    // number of bodies
    int nbod(config["bodies"].size());

    // body list
    std::vector<Body> bodies;

    // assign bodies
    for (int i=0; i<nbod; ++i) {
      bodies.push_back(Body(config["bodies"][i].as<std::string>()));
    };

    // initial and final times
    double t0(spice::mjd2000(config["t0"].as<std::string>()));
    double tf(spice::mjd2000(config["tf"].as<std::string>()));

    // initial and final states
    std::vector<double> x0(spice::state(t0, config["origin"].as<std::string>()));
    x0.push_back(sc.mass);
    std::vector<double> xf(spice::state(t0, config["target"].as<std::string>()));
    xf.push_back(config["mf"].as<double>());

    // number of control inputs
    int nin(nbod*6 + 1);

    // input regularisation vector
    std::vector<double> ref(nin);
    for (int i=0; i<nbod; ++i) {
      // we compute the initial state of the body
      const std::vector<double> sb(bodies[i].state(t0));
      // we add the relitive spacecraft state
      const std::vector<double> srel(6);
      for (int j=0; j<6; ++j) {ref[i*6 + j] = x0[j] - sb[j];};
    };
    ref[nin-1] = sc.mass;

    // relative neural controller
    Controller::Relative controller(bodies, config["netshape"].as<std::vector<int>>(), ref);

    // culminating phase
    return Phase<Controller::Relative>(sc, bodies, controller, x0, xf, t0, tf);

  };

};

#endif
