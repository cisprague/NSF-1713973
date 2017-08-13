// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp

// problems
#include <iostream>
#include <string>
#include "ptp.hpp"

/*
#include "pagmo/problem.hpp"
#include "pagmo/island.hpp"
#include "pagmo/population.hpp"
#include "pagmo/algorithms/cstrs_self_adaptive.hpp"
#include "pagmo/types.hpp"
#include "pagmo/io.hpp"
#include "pagmo/problems/unconstrain.hpp"
*/

int main(void) {

  // instantiate the problem
  PTP prob(1);


  /*
  // instantiate the problem
  Problems::PTP prob("../dta/ptp/config.yaml");
  linalg::display_vec(prob.phase.mismatch());

  // create an unconstrained pagmo problem
  pagmo::problem pagmo_problem(prob);

  pagmo_problem.set_c_tol({1e-10, 1e-10, 1e-10, 1e-10, 1e-10, 1e-10, 1e-10});

  // create population
  pagmo::population pop(pagmo_problem, 50);

  // instantiate an algorithm
  pagmo::cstrs_self_adaptive algo(1000);
  algo.set_verbosity(1);

  // instantiate island
  pop = algo.evolve(pop);

  //prob.set_decision(pop.champion_x());
  //prob.save_decision();
  //prob.phase.plot(0, 1);
  //prob.phase.plot(1, 2);
  //prob.phase.plot(0, 2);

  */


  return 0;
};
