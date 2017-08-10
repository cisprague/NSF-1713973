// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp

// problems
#include <iostream>
#include <string>
#include "ptp.hpp"
#include "pagmo/problem.hpp"
#include "pagmo/types.hpp"
#include "pagmo/io.hpp"
#include "pagmo/algorithms/sade.hpp"
#include "pagmo/problems/unconstrain.hpp"

int main(void) {
  std::string fname;
  std::cout << "Enter path name of YAML configuration file: ";
  std::getline(std::cin, fname);

  // instantiate the problem
  Problems::PTP prob(fname);

  // create an unconstrained pagmo problem
  pagmo::unconstrain pagmo_problem(prob);
  // tell me about it..
  // create population
  pagmo::population pop(pagmo_problem, 10);
  // instantiate an algorithm
  pagmo::sade algo(20);
  algo.set_verbosity(1);
  pop = algo.evolve(pop);




  prob.set_decision(pop.champion_x());
  prob.phase.plot(0, 1);
  return 0;
};
