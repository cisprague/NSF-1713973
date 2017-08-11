// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp

#ifndef ptp_hpp
#define ptp_hpp
#include <fstream>
#include <vector>
#include <string>
#include <tuple>
#include <limits>
#include <iostream>
#include <iomanip>
#include "yaml.h"

#include "../cor/spacecraft.hpp"
#include "../cor/spice.hpp"
#include "../cor/body.hpp"
#include "../cor/controller.hpp"
#include "../cor/phase.hpp"
#include "../cor/mlp.hpp"
#include "../cor/constants.hpp"

namespace Problems {

  struct PTP {

    // constants
    const std::string origin, target;
    const std::pair<std::vector<double>, std::vector<double>> bounds;

    // manipulated
    mutable Phase<Controller::Relative> phase;

    // constructor
    PTP (const std::string & fpath) :
      phase(init(fpath)),
      origin(init_org(fpath)),
      target(init_tar(fpath)),
      bounds(init_bounds(fpath)) {};

    // defaulting for PaGMO
    PTP (void) : PTP("../dta/ptp/config.yaml") {};

    // destructor
    ~PTP (void) {};

    // get current decision
    std::vector<double> get_decision (void) const {

      // create decision vector
      std::vector<double> dv;

      // boundary conditions
      dv.push_back(phase.t0);    // initial time
      dv.push_back(phase.tN);    // final time
      dv.push_back(phase.xN.at(6)); // final mass

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

    // decode decision
    std::tuple<double, double, double, std::vector<double>, std::vector<double>> decode (const std::vector<double> & decision) const {

      // boundary conditions
      double t0(decision[0]);
      double tf(decision[1]);
      double mf(decision[2]);

      // weights
      std::vector<double> weights;
      for (int i=0; i<phase.controller.mlp.wd; ++i) {
        weights.push_back(decision[3 + i]);
      };

      // biases
      std::vector<double> biases;
      for (int i=0; i<phase.controller.mlp.bd; ++i) {
        biases.push_back(decision[3 + phase.controller.mlp.wd + i]);
      };

      // initialise decision tuple
      return std::tuple<double, double, double, std::vector<double>, std::vector<double>>(t0, tf, mf, weights, biases);

    };

    // set decision
    void set_decision (const std::vector<double> & decision) const {

      // decode the vector
      std::tuple<double, double, double, std::vector<double>, std::vector<double>> soln(decode(decision));

      // parametres
      double t0(std::get<0>(soln));
      double tf(std::get<1>(soln));
      double mf(std::get<2>(soln));
      std::vector<double> weights(std::get<3>(soln));
      std::vector<double> biases(std::get<4>(soln));

      // initial state
      std::vector<double> x0(spice::state(t0, origin));
      x0.push_back(phase.spacecraft.mass);

      // terminal state
      std::vector<double> xf(spice::state(tf, target));
      xf.push_back(mf);

      // set the phase
      phase.set_phase(x0, xf, t0, tf);

      // set the weights and biases
      phase.controller.mlp.set_weights(weights);
      phase.controller.mlp.set_biases(biases);

    };

    //// pagmo ////

    // fitness
    std::vector<double> fitness (const std::vector<double> & decision) const {
      // 1) set the decision
      set_decision(decision);
      // 2) compute the objective
      double obj(-decision[2]); // final mass
      // 3) compute equality constraint
      std::vector<double> eq(phase.mismatch());
      // 4) assemble fitness vector
      std::vector<double> fit{obj};
      for (int i=0; i<7; ++i) {fit.push_back(eq.at(i));}
      linalg::display_vec(fit);
      return fit;
    };

    // bounds
    std::pair<std::vector<double>, std::vector<double>> get_bounds (void) const {
      return bounds;
    };

    // nobj
    std::vector<double>::size_type get_nobj (void) const {return 1;}

    // nec
    std::vector<double>::size_type get_nec (void) const {return 7;}

    // save a decision
    void save_decision (const std::vector<double> & dec) const {
      // create decision file
      std::ofstream f("../dta/ptp/dec.yaml");
      // header
      f << "# Christopher Iliffe Sprague\n";
      f << "# cisprague@ac.jaxa.jp\n\n";
      f << "# Point to point (PTP) trajectory optimisation problem decision vector.\n";
      f << "---\n\n";
      f << "decision:\n";
      for (int i=0; i<dec.size(); ++i) {
        if (i == 0) {
          f << "\n  # initial time [mjd2000]\n";
        };
        if (i == 1) {
          f << "\n  # final time [mjd2000]\n";
        };
        if (i == 2) {
          f << "\n  # final mass [kg]\n";
        }
        if (i == 3) {
          f << "\n  # weights and biases\n";
        };
        f << "  - ";
        f << std::setprecision(std::numeric_limits<double>::digits);
        f << dec.at(i);
        f << "\n";
      };
      f.close();
    };

    // save the decision
    void save_decision (void) const {
      // save the decision to file
      save_decision(get_decision());
    };

    // load decision
    void load_decision (void) {
      // load yaml configuration file
      YAML::Node dec(YAML::LoadFile("../dta/ptp/dec.yaml"));
      // set the decision
      set_decision(dec["decision"].as<std::vector<double>>());
    };

    //// initialisers ////
    private:

      // phase initialiser
      static Phase<Controller::Relative> init (const std::string & fpath) {

        // load configuration file
        YAML::Node config(YAML::LoadFile(fpath));

        // spacecraft
        const double mass(config["mass"].as<double>());
        const double thrust(config["thrust"].as<double>());
        const double isp(config["isp"].as<double>());
        const Spacecraft sc(mass, thrust, isp);

        // load kernels
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
        std::vector<double> xf(spice::state(tf, config["target"].as<std::string>()));
        xf.push_back(config["mf"].as<double>());

        // input regularisation vector
        std::vector<double> ref;
        for (int i=0; i<nbod; ++i) {
          for (int j=0; j<3; ++j) {ref.push_back(R_EARTH);};
          for (int j=0; j<3; ++j) {ref.push_back(V_EARTH);};
        };
        ref.push_back(sc.mass);

        // relative neural controller
        Controller::Relative controller(bodies, config["net"].as<std::vector<int>>(), ref);

        // culminating phase
        return Phase<Controller::Relative>(sc, bodies, controller, x0, xf, t0, tf);

      };

      // init origin
      static std::string init_org (const std::string & fpath) {
        YAML::Node config(YAML::LoadFile(fpath));
        return config["origin"].as<std::string>();
      };

      // init target
      static std::string init_tar (const std::string & fpath) {
        YAML::Node config(YAML::LoadFile(fpath));
        return config["target"].as<std::string>();
      };

      // init bounds
      static std::pair<std::vector<double>, std::vector<double>> init_bounds (const std::string & fpath) {

        // load parametres
        YAML::Node config(YAML::LoadFile(fpath));

        // load kernels
        spice::load_kernels();

        // create lower bound
        std::vector<double> lb{
          spice::mjd2000(config["t0lb"].as<std::string>()),
          spice::mjd2000(config["tflb"].as<std::string>()),
          0
        };

        // create upper bound
        std::vector<double> ub {
          spice::mjd2000(config["t0ub"].as<std::string>()),
          spice::mjd2000(config["tfub"].as<std::string>()),
          config["mass"].as<double>()
        };

        // number of network inputs
        int ni(config["bodies"].size()*6 + 1);

        // create network shape
        std::vector<int> shape(ML::MLP::full_struct(ni, 3, config["net"].as<std::vector<int>>()));

        // get weight and bias parametre dimensions
        int wd(ML::MLP::weight_dim(shape));
        int bd(ML::MLP::bias_dim(shape));

        // append weight and bias bounds
        for (int i=0; i<wd; ++i) {lb.push_back(-2); ub.push_back(2);}
        for (int i=0; i<bd; ++i) {lb.push_back(-2); ub.push_back(2);}

        // create the pair
        return std::pair<std::vector<double>, std::vector<double>>(lb, ub);

      };



  };

};

#endif
