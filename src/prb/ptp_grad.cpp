// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp

#include <iostream>
#include <string>
#include <pagmo/problem.hpp>
#include <pagmo/population.hpp>
#include <pagmo/algorithms/mbh.hpp>
#include <pagmo/algorithms/ipopt.hpp>
#include <yaml.h>
#include "ptp.hpp"
#include "../cor/spice.hpp"

int main(void) {

  // configurations
  YAML::Node config(YAML::LoadFile("ptp.yaml"));
  const int nind(config["nind"].as<int>());
  const int ngen(config["ngen"].as<int>());
  const double etol(config["etol"].as<double>());
  const double nevo(config["nevo"].as<int>());

  // load kernels
  spice::load_kernels();

  // instatiate native problem
  PTP prob(1);

  // instantiate problem
  pagmo::problem pgprob(prob);

  // instantiate inner algorithm
  pagmo::ipopt ialgo;
  ialgo.set_verbosity(5);

  // instantiate outer algorithm
  pagmo::mbh algo(ialgo, 5, 1e-2);

  // population of decisions
  pagmo::population pop(pgprob, nind);

  // evolve
  for (int i=0; i<nevo; ++i) {
    pop = algo.evolve(pop);
    // decision vectors
    const std::vector<std::vector<double>> dvs{pop.champion_x()};
    // save decisions
    prob.save(dvs, "ptp_grad_dec.yaml");
  };


  return 0;
};
