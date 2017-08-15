#ifndef ptp_hpp
#define ptp_hpp
#include <tuple>
#include <string>
#include <vector>
#include <iostream>
#include <yaml.h>
#include <cmath>
#include <fstream>
#include <matplotlibcpp.h>
#include "../cor/spacecraft.hpp"
#include "../cor/body.hpp"
#include "../cor/phase.hpp"
#include "../cor/controller.hpp"
#include "../cor/spice.hpp"
#include "../cor/propagator.hpp"

struct PTP {

  // a priori
  const std::string origin, target;
  const std::pair<std::vector<double>, std::vector<double>> bounds;

  // a posteriori
  mutable Phase phase;
  mutable Controller controller;

  // constructor
  PTP (const int & i) :
    origin(init_origin()),
    target(init_target()),
    phase(init_phase()),
    controller(init_controller()),
    bounds(init_bounds()) {
  };

  // constructor
  PTP (void) : PTP(1) {};

  // destructor
  ~PTP (void) {};

  // plot
  void plot (void) const {
    // get trajectories
    const std::pair<Propagator, Propagator> traj(phase.mismatch_trajectory(controller));
    // states
    matplotlibcpp::plot(traj.first.states[0], traj.first.states[1], "g");
    matplotlibcpp::plot(traj.second.states[0], traj.second.states[1], "b");
    matplotlibcpp::show();
    matplotlibcpp::plot(traj.first.states[1], traj.first.states[2], "g");
    matplotlibcpp::plot(traj.second.states[1], traj.second.states[2], "b");
    matplotlibcpp::show();
    matplotlibcpp::plot(traj.first.states[0], traj.first.states[2], "g");
    matplotlibcpp::plot(traj.second.states[0], traj.second.states[2], "b");
    matplotlibcpp::show();
    matplotlibcpp::plot(traj.first.times, traj.first.states[6], "g");
    matplotlibcpp::plot(traj.second.times, traj.second.states[6], "b");
    matplotlibcpp::show();

    // number of forward and backward points
    const int npf(traj.first.times.size()), npb(traj.second.times.size());
    // control norm vectors
    std::vector<double> f(npf), b(npb);
    for (int i=0; i<npf; ++i) {
      f[i] = 0;
      for (int j=0; j<3; ++j) {f[i] += pow(traj.first.controls[j][i], 2);};
      f[i] = sqrt(f[i]);
    };
    for (int i=0; i<npb; ++i) {
      b[i] = 0;
      for (int j=0; j<3; ++j) {b[i] += pow(traj.first.controls[j][i], 2);};
      b[i] = sqrt(b[i]);
    };
    matplotlibcpp::plot(traj.first.times, traj.first.controls[0], "g");
    matplotlibcpp::plot(traj.second.times, traj.second.controls[0], "b");
    matplotlibcpp::plot(traj.first.times, traj.first.controls[1], "g");
    matplotlibcpp::plot(traj.second.times, traj.second.controls[1], "b");
    matplotlibcpp::plot(traj.first.times, traj.first.controls[2], "g");
    matplotlibcpp::plot(traj.second.times, traj.second.controls[2], "b");
    matplotlibcpp::plot(traj.first.times, f, "k.");
    matplotlibcpp::plot(traj.second.times, b, "k.");
    matplotlibcpp::show();
  };

  // save decision
  void save(const std::vector<double> & dv) const {
    std::string info;
    info.append("Point to point (PTP) trajectory optimisation problem\n");
    info.append("decision vector with 3 phase parametres\n");
    info.append("and a neural network with ");
    info.append(std::to_string(controller.nw));
    info.append(" weights and ");
    info.append(std::to_string(controller.nb));
    info.append(" biases.");
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Comment(info);
    out << YAML::Key << "t0";
    out << YAML::Value << dv[0];
    out << YAML::Key << "tf";
    out << YAML::Value << dv[1];
    out << YAML::Key << "mf";
    out << YAML::Value << dv[2];
    out << YAML::Key << "weights";
    out << YAML::Value << YAML::BeginSeq;
    for (int i=0; i<controller.nw; ++i) {
      out << dv[3 + i];
    };
    out << YAML::EndSeq;
    out << YAML::Key << "biases";
    out << YAML::Value << YAML::BeginSeq;
    for (int i=0; i<controller.nb; ++i) {
      out << dv[3+ controller.nw + i];
    };
    out << YAML::EndSeq << YAML::EndMap;
    std::ofstream fout("ptp_dec.yaml");
    fout << out.c_str();
  };

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

  // fitness
  std::vector<double> fitness (const std::vector<double> & dv) const {
    // 1) set the phase and controller
    set(dv);
    // 2) compute the objective
    std::vector<double> fit{-dv[2]};
    // 3) compute the equality constraints
    std::vector<double> ceq(phase.mismatch(controller));
    fit.insert(fit.end(), ceq.begin(), ceq.end());
    std::cout << "[ ";
    for (int i=0; i<8; ++i) {
      std::cout << fit[i] << " ";
    };
    std::cout << "]" << std::endl;
    return fit;
  };

  // bounds
  std::pair<std::vector<double>, std::vector<double>> get_bounds (void) const {
    return bounds;
  };

  // number of objectives
  std::vector<double>::size_type get_nobj (void) const {
    return 1;
  };

  // number of equality constraints
  std::vector<double>::size_type get_nec (void) const {
    return 7;
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
    static Controller init_controller (void)  {
      // load configuration
      YAML::Node config(YAML::LoadFile("ptp.yaml"));
      // hidden shape of neural network
      const std::vector<int> hshape(config["net"].as<std::vector<int>>());
      // spacecraft and bodies
      const Phase p(init_phase());
      // instantiate controller
      return Controller(hshape, p.spacecraft, p.bodies);
    };

    // initialise bounds
    static std::pair<std::vector<double>, std::vector<double>> init_bounds (void) {
      // load configuration
      YAML::Node config(YAML::LoadFile("ptp.yaml"));
      // start with phase parametres
      std::pair<std::vector<double>, std::vector<double>> biases_(
        {
          spice::mjd2000(config["t0lb"].as<std::string>()),
          spice::mjd2000(config["tflb"].as<std::string>()),
          config["mlb"].as<double>()
        },
        {
          spice::mjd2000(config["t0ub"].as<std::string>()),
          spice::mjd2000(config["tfub"].as<std::string>()),
          config["m0"].as<double>()
        }
      );
      // instantiate the neural network
      Controller c(init_controller());
      // problem dimensions
      const int dim(3 + c.nw + c.nb);
      // allocate space
      biases_.first.reserve(dim);
      biases_.second.reserve(dim);
      // get weight and bias bounds
      const int w(config["w"].as<double>());
      const int b(config["b"].as<double>());
      // append weights
      for (int i=0; i<c.nw; ++i) {
        biases_.first.push_back(-w);
        biases_.second.push_back(w);
      };
      // append biases
      for (int i=0; i<c.nb; ++i) {
        biases_.first.push_back(-b);
        biases_.second.push_back(b);
      };
      // make sure bounds are consistant
      if (biases_.first.size() != biases_.second.size()) {
        throw "Bounds dimensions must match.";
      };
      // make sure bounda are long enough
      if (biases_.first.size() != dim) {
        throw "Bounds must match problem dimensions.";
      };
      return biases_;
    };

};


#endif
