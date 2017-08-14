#ifndef ptp_hpp
#define ptp_hpp
#include <tuple>
#include <string>
#include <vector>
#include <iostream>
#include <yaml.h>
#include "../cor/spacecraft.hpp"
#include "../cor/body.hpp"
#include "../cor/phase.hpp"
#include "../cor/controller.hpp"
#include "../cor/spice.hpp"

struct PTP {

  // a priori
  const std::string origin, target;
  //const std::pair<std::vector<double>, std::vector<double>> bounds;

  // a posteriori
  mutable Phase phase;
  mutable Controller controller;

  // constructor
  PTP (const int & i) :
    origin(init_origin()),
    target(init_target()),
    phase(init_phase()),
    controller(init_controller())
    //bounds(init_bounds())
    {
      std::cout << origin << std::endl;
      std::cout << target << std::endl;
      std::cout << phase.t0 << std::endl;
      std::cout << phase.tf << std::endl;
      std::cout << phase.spacecraft.mass << std::endl;
    };

  // destructor
  ~PTP (void) {};

  // set problem
  void set (const std::vector<double> & dv) const {
    // validate decision vector
    if (dv.size() != 3 + controller.nw + controller.nb) {
      throw "Decision vector dimensions are incorrect.";
    };
    // initial and final epochs
    const double t0(dv[0]), tf(dv[1]);
    // initial state
    std::vector<double> x0(spice::state(t0, origin));
    x0.push_back(phase.spacecraft.mass);
    // final state
    std::vector<double> xf(spice::state(tf, target));
    xf.push_back(dv[2]);
    // set the phase
    phase.set(x0, xf, t0, tf);
    // instantiate neural network weights vector
    std::vector<double> w;
    // append each weight
    for (int i=0; i<controller.nw; ++i) {
      w.push_back(dv[3 + i]);
    };
    // instantiate neural network bias vector
    std::vector<double> b;
    // append each bias
    for (int i=0; i<controller.nb; ++i) {
      b.push_back(dv[3 + controller.nw + i]);
    };
    // set neural network weights and biases
    controller.set_weights(w);
    controller.set_biases(b);
  };

  // problem name
  std::string get_name (void) const {
    std::string name;
    name.append("Point to point (PTP) trajectory optimisation problem from ");
    name.append("NAIF ID ").append(origin).append(" to ");
    name.append("NAIF ID ").append(target);
    return name;
  };

  private:

    // initialise origin
    static std::string init_origin (void) {
      // load configuration
      YAML::Node config(YAML::LoadFile("ptp.yaml"));
      return config["origin"].as<std::string>();
    };

    // initialise target
    static std::string init_target (void) {
      // load configuration
      YAML::Node config(YAML::LoadFile("ptp.yaml"));
      return config["target"].as<std::string>();
    };

    // initialise phase
    static Phase init_phase (void) {
      // load configuration
      YAML::Node config(YAML::LoadFile("ptp.yaml"));
      // instantiate spacecraft
      const Spacecraft spacecraft(
        config["m0"].as<double>(),
        config["T"].as<double>(),
        config["isp"].as<double>()
      );
      // instantiate bodies
      const std::vector<Body> bodies(Body::vector(config["bodies"].as<std::vector<std::string>>()));
      // initial and final times
      const double t0(spice::mjd2000(config["t0"].as<std::string>()));
      const double tf(spice::mjd2000(config["tf"].as<std::string>()));
      // initial state
      std::vector<double> x0(spice::state(t0, config["origin"].as<std::string>()));
      x0.push_back(spacecraft.mass);
      // final state
      std::vector<double> xf(spice::state(tf, config["target"].as<std::string>()));
      xf.push_back(config["mf"].as<double>());
      // instantiate the culminating phase
      return Phase(spacecraft, bodies, t0, tf, x0, xf);
    };

    // initialise controller
    Controller init_controller (void) const {
      // load configuration
      YAML::Node config(YAML::LoadFile("ptp.yaml"));
      // hidden shape of neural network
      const std::vector<int> hshape(config["net"].as<std::vector<int>>());
      // spacecraft
      const Spacecraft spacecraft(
        config["m0"].as<double>(),
        config["T"].as<double>(),
        config["isp"].as<double>()
      );
      // instantiate bodies
      const std::vector<Body> bodies(Body::vector(config["bodies"].as<std::vector<std::string>>()));
      // instantiate controller
      return Controller(hshape, spacecraft, bodies);
    };

    /*/ initialise bounds
    std::pair<std::vector<double>, std::vector<double>> init_bounds (void) const {
      // number of NLP variables
      const int dim(3 + controller.nw + controller.nb);
      std::cout << dim << std::endl;
      // allocate bounds tuple
      std::pair<std::vector<double>, std::vector<double>> b;
      b.first.reserve(dim);
      b.second.reserve(dim);
      // load configuration
      YAML::Node config(YAML::LoadFile("ptp.yaml"));
      // phase lower bounds
      b.first.push_back(spice::mjd2000(config["t0lb"].as<std::string>()));
      b.first.push_back(spice::mjd2000(config["tflb"].as<std::string>()));
      b.first.push_back(config["mlb"].as<double>());
      // phase upper bounds
      b.second.push_back(spice::mjd2000(config["t0ub"].as<std::string>()));
      b.second.push_back(spice::mjd2000(config["tfub"].as<std::string>()));
      b.second.push_back(config["m0"].as<double>());
      // neural network weight bounds
      const double weight(config["w"].as<double>());
      const double bias(config["b"].as<double>());
      // weight bounds
      for (int i=0; i<controller.nw; ++i) {
        b.first.push_back(-weight);
        b.second.push_back(weight);
      };
      // bias bounds
      for (int i=0; i<controller.nb; ++i) {
        b.first.push_back(-bias);
        b.second.push_back(bias);
      };
      // make sure bounds are consistant
      if (b.first.size() != b.second.size()) {throw "Bounds must be equal in dimension";};
      return b;
    };
    */

};


#endif
