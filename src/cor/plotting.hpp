// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp

#ifndef plotting_hpp
#define plotting_hpp
#include <string>
#include "matplotlibcpp.h"
#include "body.hpp"
#include "propagator.hpp"
#include "linalg.hpp"
#include "spice.hpp"

// 1 spacecraft
void plot_traj (
  const int                 & xi,
  const int                 & yi,
  const Propagator::Results & results,
  const std::string         & persp = "SSB"
) {

  // number of points
  const int npts(results.times.size());

  // allot the trajectory
  std::vector<std::vector<double>> traj(7, std::vector<double>(npts));


  // compute plotting pts
  for (int i=0; i<npts; ++i) {
    // compute the time
    const double t(results.times[i]);
    // compute state of the origin
    const std::vector<double> sb(spice::state(t, persp));
    // compute state of spacecraft
    const std::vector<double> ss(results.states[i]);
    // compute the relative spacecraft state
    for (int j=0; j<7; ++j) {
      // assign relative state to trajectory
      traj[j][i] = ss[j] - sb[j];
    };
  };

  // plot the trajectory
  matplotlibcpp::plot(traj[xi], traj[yi], "k");
};

// N spacecraft
void plot_traj (
  const int                              & xi,
  const int                              & yi,
  const std::vector<Propagator::Results> & results,
  const std::string                      & persp = "SSB"
) {

  // number of spacecraft
  const int nsc(results.size());

  // plot each spacecraft
  for (int i=0; i<nsc; ++i) {plot_traj(xi, yi, results[i], persp);};
};

// 1 body, 1 time sequence
void plot_traj (
  const int                 & xi,
  const int                 & yi,
  const std::vector<double> & times,
  const Body                & body,
  const std::string         & persp = "SSB"
) {

  // number of points
  const int npts(times.size());

  // allot the trajectory
  std::vector<std::vector<double>> traj(6, std::vector<double>(npts));

  // compute the trajectory
  for (int i=0; i<npts; ++i) {
    // compute relative body position
    const std::vector<double> sb(body.state(times[i], persp));
    // assign trajectory
    for (int j=0; j<6; ++j) {
      traj[j][i] = sb[j];
    };
  };

  // plot the trajectory
  matplotlibcpp::plot(traj[xi], traj[yi], "k,");
};

// N bodies, 1 time sequence
void plot_traj (
  const int                 & xi,
  const int                 & yi,
  const std::vector<double> & times,
  const std::vector<Body>   & bodies,
  const std::string         & persp = "SSB"
) {
  // number of bodies
  const int nbod(bodies.size());
  // plot each body
  for (int i=0; i<nbod; ++i) {plot_traj(xi, yi, times, bodies[i], persp);};
};

// 1 or N bodies, N time sequences
template <typename B>
void plot_traj (
  const int                 & xi,
  const int                 & yi,
  const std::vector<std::vector<double>> & times,
  const B                   & bodies,
  const std::string         & persp = "SSB"
) {
  // number of time sequences
  const int nseq(times.size());
  // plot each sequence
  for (int i=0; i<nseq; ++i) {plot_traj(xi, yi, times[i], bodies, persp);};
};

// 1 or N bodies, 1 results
template <typename B>
void plot_traj (
  const int                 & xi,
  const int                 & yi,
  const Propagator::Results & results,
  const B                   & bodies,
  const std::string         & persp = "SSB"
) {
  // plot bodies according to time
  plot_traj(xi, yi, results.times, bodies, persp);
  // plot spacecraft
  plot_traj(xi, yi, results, persp);
};

// 1 or N bodies, M results
template <typename B>
void plot_traj (
  const int                              & xi,
  const int                              & yi,
  const std::vector<Propagator::Results> & results,
  const B                                & bodies,
  const std::string                      & persp = "SSB"
) {
  // number of time sequences
  const int nseq(results.size());
  // plot bodies according to time
  for (int i=0; i<nseq; ++i){
    plot_traj(xi, yi, results[i].times, bodies, persp);
  };
  // plot spacecraft
  plot_traj(xi, yi, results, persp);
};

void plot_show (void) {matplotlibcpp::show();};

#endif
