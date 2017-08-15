// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp

// problems
#include <iostream>
#include <string>
#include <pagmo/problem.hpp>
#include <pagmo/island.hpp>
#include <pagmo/population.hpp>
#include <pagmo/algorithms/cstrs_self_adaptive.hpp>

#include "ptp.hpp"
#include "../cor/spice.hpp"

int main(void) {

  // instatiate native problem
  PTP prob(1);

  // instantiate problem
  pagmo::problem pgprob(prob);

  // set constraint tolerance
  pgprob.set_c_tol({1e-8, 1e-8, 1e-8, 1e-8, 1e-8, 1e-8, 1e-8});

  // create population
  pagmo::population pop(pgprob, 50);

  // instantiate an algorithm
  pagmo::cstrs_self_adaptive algo(1000);
  algo.set_verbosity(1);

  // instantiate island
  //pagmo::island isl(algo, pop);

  // evolve the island
  //isl.evolve(1);
  pop = algo.evolve(pop);

  // get the champion decision
  //const std::vector<double> dec(isl.get_population().champion_x());
  const std::vector<double> dec(pop.champion_x());

  // save the decision
  prob.save(dec);

  // set the problem decision
  prob.set(dec);

  // plot the decision
  prob.plot();



  return 0;
};
