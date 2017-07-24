// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp
// NSF Award 1713973

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
