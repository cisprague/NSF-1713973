// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp

#ifndef epo_hpp
#define epo_hpp

#include <string>
#include "SpiceUsr.h"

class Epoch {

  public:

    // members
    double mjd2000;

    // constructor
    Epoch(int y_, int m_, int d_, int h_, int s_, int ms_) {};

    // destructor
    ~Epoch(void) {};
};

#endif
