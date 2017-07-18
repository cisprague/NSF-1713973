#ifndef mission_hpp
#define mission_hpp
#include <vector>
#include "spacecraft.hpp"
#include "body.hpp"
#include "phase.hpp"

struct Mission {

  // a priori
  Spacecraft spacecraft;
  std::vector<Phase> phases;



};

#endif
