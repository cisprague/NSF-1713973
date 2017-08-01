// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp

#ifndef ptp_hpp
#define ptp_hpp
#include <vector>
#include <string>
#include "body.hpp"
#include "phase.hpp"
#include "spacecraft.hpp"
#include "spice.hpp"

struct PTP {

  // members
  Phase phase;

  // set up problem
  PTP (
    const std::string & org_,                 // origin body
    const std::string & tar_,                 // target body
    const double & mass_,                     // wet mass
    const double & thrust_,                   // max thrust
    const double & isp_,                      // specific impulse
    const std::vector<std::string> & bodies_, // list of body names
    const std::vector<int> & hshape_,         // hidden network shape
    const std::string & t0_,                  // initial time guess
    const std::string & tf_,                  // final time guess
    const double & mf_                        // final mass guess
  ) {};

  private:

    // set up problem
    Phase init (
      const double & mass_,                     // wet mass
      const double & thrust_,                   // max thrust
      const double & isp_,                      // specific impulse
      const std::vector<std::string> & bodies_, // list of body names
      const std::vector<int> & hshape_,         // hidden network shape
      const std::string & t0_,                  // initial time guess
      const std::string & tf_,                  // final time guess
      const double & mf_                        // final mass guess
    ) {

      // load ephemerides
      spice::load_kernels();
      // number of bodies
      int nbod(bodies_.size());
      // body list
      std::vector<Body> bodies(nbod);
      // assign bodies
      for (int i=0; i<nbod; ++i) {bodies[i] = Body(bodies_[i]);};
      // inital and final time
      double t0(spice::mjd2000(t0));
      double tN(spice::mjd2000(tN));
      //

      // NOTE: finish setting up Pagmo problem



    };

};


#endif
