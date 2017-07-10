// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp

#ifndef msn_hpp
#define msn_hpp

#include <vector>
#include <iostream>

#include "../cor/phs.hpp"
#include "../cor/spc.hpp"

// phased mission
class Mission {

  public:

    // types
    typedef std::vector<Phase> Phases;

    // members
    Spacecraft & spacecraft; // spacecraft undertaking mission
    Phases     & phases;     // trajectory phases

    // constructor
    Mission(
      Spacecraft & sc_,
      Phases & phases_
    ) : spacecraft(sc_), phases(phases_) {
      cout << phases.size() << " phase mission " << this;
      cout << " constructed with spacecraft " << &spacecraft;
      cout << "\n" << endl;
    };

    // desctructor
    ~Mission(void) {
      cout << "Mission " << this << " desctructed\n" << endl;
    };

};

#endif
