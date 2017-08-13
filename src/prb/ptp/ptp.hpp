#ifndef ptp_hpp
#define ptp_hpp
#include <tuple>
#include <string>
#include <iostream>
#include <yaml.h>
#include "../../cor/phase.hpp"
#include "../../cor/controller.hpp"
#include "../../cor/spice.hpp"

struct PTP {

  // a priori
  const std::string origin, target;
  //const std::pair<std::vector<double>, std::vector<double>> bounds;

  // a posteriori
  //mutable Phase phase;
  //mutable Controller controller;

  // constructor
  PTP (int i) {std::cout << "hello" << std::endl;};

  // destructor
  ~PTP (void) {};

  /*
  // initialise origin
  static std::string init_origin (void) {
    // load configuration
    YAML::Node config(YAML::LoadFile("config.yaml"));
    return config["origin"].as<std::string>();
  };

  // initialise target
  static std::string init_target (void) {
    // load configuration
    YAML::Node config(YAML::LoadFile("config.yaml"));
    return config["target"].as<std::string>();
  };
  */

  /*
  // initialise bounds
  static std::pair<std::vector<double>, std::vector<double>> init_bounds (void) {
    // load configuration
    YAML::Node config(YAML::LoadFile("config.yaml"));
    // phase lower bounds
    std::vector<double> lb{
      spice::mjd2000(config["t0lb"].as<std::string>()),
      spice::mjd2000(config["tflb"].as<std::string>()),
      config["mlb"]
    };
    // phase upper bounds
    std::vector<double> ub{
      spice::mjd2000(config["t0ub"].as<std::string>()),
      spice::mjd2000(config["tfub"].as<std::string>()),
      config["m0"].as<double>()
    };
    // weight bounds
    for (int i=0; i<controller.nw; ++i) {
      lb.push_back(config["wlb"].as<double>());
      ub.push_back(config["wub"].as<double>());
    };
    // bias bounds
    for (int i=0; i<controller.nb; ++i) {
      lb.push_back(config["blb"].as<double>());
      ub.push_back(config["bub"].as<double>());
    };
    // assemble the pair
    return std::pair<std::vector<double>, std::vector<double>>(lb, ub);
  };
  */

};


#endif
