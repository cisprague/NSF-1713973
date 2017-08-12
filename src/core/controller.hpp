#ifndef controller_hpp
#define controller_hpp
#include <vector>
#include <random>
#include <cmath>
#include "spacecraft.hpp"
#include "body.hpp"
#include "constants.hpp"

struct Controller {

  // a priori
  const Spacecraft spacecraft;
  const std::vector<Body> bodies;
  const std::vector<int> shape;
  const int nl, ni, no, nw, nb;

  // a posteriori
  std::vector<std::vector<std::vector<double>>> weights;
  std::vector<std::vector<double>> biases;

  // constructor
  Controller (
    const std::vector<int> & hshape_,
    const Spacecraft & spacecraft_,
    const std::vector<Body> & bodies_
  ) :
    spacecraft(spacecraft_),
    bodies(bodies_),
    shape(init_shape(hshape_, bodies_.size())),
    weights(init_weights(shape)),
    biases(init_biases(shape)),
    nw(init_weight_dim(shape)),
    nb(init_bias_dim(shape)),
    nl(shape.size()),
    ni(shape.front()),
    no(shape.back())
  {};

  // initialise network shape
  static std::vector<int> init_shape(
    const std::vector<int> & hshape_,
    const int & nbodies_
  ) {
    if (hshape_.size() == 0 || nbodies_ == 0) {
      throw "Must have bodies and layers.";
    };
    // number of hidden layers
    const int nh(hshape_.size());
    // number of inputs
    const int ni(6*nbodies_ + 1);
    // shape vector
    std::vector<int> shape{ni};
    // add hidden layers
    for (int i=0; i<nh; ++i) {
      shape.push_back(hshape_.at(i));
    };
    // add output layer
    shape.push_back(3);
    return shape;
  };

  // initialise weights
  static std::vector<std::vector<std::vector<double>>> init_weights (
    const std::vector<int> & shape_
  ) {
    // number of weights matrices
    const int nw(shape_.size() - 1);
    // initialise weights
    std::vector<std::vector<std::vector<double>>> w(nw);
    // random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    // standard normal distribution
    std::normal_distribution<double> d(0, 1);
    // initialise columns
    for (int i=0; i<nw; ++i) {
      w.at(i) = std::vector<std::vector<double>>(shape_.at(i+1));
      // initialise rows
      for (int j=0; j<shape_.at(i+1); ++j) {
        w.at(i).at(j) = std::vector<double>(shape_.at(i));
        // for each elements
        for (int k=0; k<shape_.at(i); ++k) {
          w.at(i).at(j).at(k) = d(gen)*sqrt(2.0/shape_.at(i));
        };
      };
    };
    return w;
  };

  // initialise biases
  static std::vector<std::vector<double>> init_biases (
    const std::vector<int> & shape_
  ) {
    // number of weights matrices
    const int nb(shape_.size() - 1);
    // initialise weights
    std::vector<std::vector<double>> b(nb);
    // random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    // standard normal distribution
    std::normal_distribution<double> d(0, 1);
    // initialise columns
    for (int i=0; i<nb; ++i) {
      b.at(i) = std::vector<double>(shape_.at(i+1));
      // initialise rows
      for (int j=0; j<shape_.at(i+1); ++j) {
        b.at(i).at(j) = d(gen)*sqrt(2.0/shape_.at(i));
      };
    };
    return b;
  };

  // initialise weight parametre dimension
  static int init_weight_dim (const std::vector<int> & shape_) {
    // initialise weight counter
    int nw(0);
    // number of weight matrices
    const int nl(shape_.size() - 1);
    // for each layer
    for (int i=0; i<nl; ++i) {
      nw += shape_.at(i)*shape_.at(i+1);
    };
    return nw;
  };

  // initialise bias parametre dimension
  static int init_bias_dim (const std::vector<int> & shape_) {
    // initialise biase counter
    int nb(0);
    // number of bias vectors
    const int nl(shape_.size() - 1);
    // for each layer
    for (int i=0; i<nl; ++i) {
      nb += shape_.at(i+1);
    };
    return nb;
  };

  // normalise inputs
  std::vector<double> normalise (
    const std::vector<double> & state_,
    const double & time_
  ) const {
    // check state validity
    if (state_.size() != 7) {throw "State must be 7D.";};
    // begin the input vector with normalised mass
    std::vector<double> input{state_.at(6)/spacecraft.mass};
    // number of bodies
    const int nbod(bodies.size());
    // for each body
    for (int i=0; i<nbod; ++i) {
      // compute the state of body at this time
      const std::vector<double> sb(bodies.at(i).state(time_));
      // compute normalised relative states
      for (int j=0; j<3; ++j) {
        input.push_back((state_.at(j) - sb.at(j))/R_EARTH);
      };
      for (int j=3; j<6; ++j) {
        input.push_back((state_.at(j) - sb.at(j))/V_EARTH);
      };
    };
    // make sure the input vector is good
    if (input.size() != ni) {throw "Input vector not sized correctly.";};
    return input;
  };

  // compute output
  std::vector<double> operator () (
    const std::vector<double> & state_,
    const double & time_
  ) const {
    // get input
    std::vector<double> in(normalise(state_, time_));
    // for each weight and biase layer
    for (int i=0; i<nl-1; ++i) {
      // number of matrix rows
      const int N(weights.at(i).size());
      // number of vector rows
      const int M(in.size());
      // make sure matrix mult is possible
      for (int j=0; j<N; ++j) {
        // number of columns in this row
        const int ncol(weights.at(i).at(j).size());
        // number of columns must match vector size
        if (ncol != M) {
          std::cout << "At layer: " << i << std::endl;
          std::cout << ncol << " != " << M << std::endl;
          throw "Number of matrix columns must match vector size;";
        };
      };
      // create product vector
      std::vector<double> prod(N);
      // for each row
      for (int j=0; j<N; ++j) {
        // for each column
        for (int k=0; k<M; ++k) {
          prod.at(j) += weights.at(i).at(j).at(k)*in.at(k);
        };
      };
      // set input
      in = prod;
      // check addition validity
      if (in.size() != biases.at(i).size()) {
        throw "Vector sizes must match.";
      };
      // add biases
      for (int j=0; j<in.size(); ++j) {
        in.at(j) += biases.at(i).at(j);
      };
      // apply activation function
      for (int j=0; j<in.size(); ++j) {
        in.at(j) = 1.0/(1.0 + exp(-in.at(j)));
      };
    };
    // make sure output vector is valid
    if (in.size() != 3) {
      throw "Neural network output vector invalid.";
    };
    // compute throttle vector from output
    const double mag(in.at(0)); // 0 to 1
    const double theta(in.at(1)*2*M_PI); // 0 to 2pi
    const double z(in.at(2)*2 - 1); // -1 to 1
    in.at(0) = (sqrt(1-pow(z,2))*cos(theta));
    in.at(1) = (sqrt(1-pow(z,2))*sin(theta));
    in.at(2) = z;
    // make sure unit vector is valid
    const float uvmag(sqrt(pow(in.at(0), 2) + pow(in.at(1), 2) + pow(in.at(2), 2)));
    if (uvmag != 1) {throw "Unit vector norm != 1.";};
    // apply throttle and max thrust
    for (int i=0; i<3; ++i) {
      in.at(i) *= mag;
    };
    return in;
  };

  // set weights
  void set_weights (const std::vector<double> & w_) {
    // ensure correct weight dimension
    if (w_.size() != nw) {
      throw "Invalid number of weights";
    };
    // counter is easier
    int count(0);
    // for each layer
    for (int i=0; i<nl-1; ++i) {
      // for each matrix row
      for (int j=0; j<shape.at(i+1); ++j) {
        // for each matrix column
        for (int k=0; k<shape.at(i); ++k) {
          weights.at(i).at(j).at(k) = w_.at(count);
          ++count;
        };
      };
    };
  };

  // set biases
  void set_biases (const std::vector<double> & b_) {
    // ensure correct weight dimension
    if (b_.size() != nb) {
      throw "Invalid number of biases.";
    };
    // counter is easier
    int count(0);
    // for each layer
    for (int i=0; i<nl-1; ++i) {
      // for each vector row
      for (int j=0; j<shape.at(i+1); ++j) {
        biases.at(i).at(j) = b_.at(count);
        ++count;
      };
    };
  };


};

#endif
