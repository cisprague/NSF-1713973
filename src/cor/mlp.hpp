// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp
// NSF Award 1713973

#ifndef mlp_hpp
#define mlp_hpp
#include <vector>
#include <random>
#include <cmath>
#include "vectools.hpp"

// machine learning
namespace ML {

  // multilayer perceptron
  struct MLP {

    // a posteriori
    std::vector<std::vector<std::vector<double>>> w; // weights
    std::vector<std::vector<double>>              b; // biases

    // constructor
    MLP (const std::vector<int> & shape) :
      w(shape.size()-1), b(shape.size()-1) {

      // initialise the random number generator
      std::random_device rd;
      std::mt19937 gen(rd());

      // standard normal distribution
      std::normal_distribution<double> d(0, 1);

      // initialise columns of weight matricies and biases
      for (int i=0; i<shape.size()-1; ++i) {
        w[i] = std::vector<std::vector<double>>(shape[i+1]);
        b[i] = std::vector<double>(shape[i+1]);

        // initialise rows of weight matricies https://arxiv.org/abs/1502.01852
        for (int j=0; j<w[i].size(); ++j) {
          w[i][j] = std::vector<double>(shape[i]);
          b[i][j] = d(gen)*sqrt(2.0/shape[i]);
          for (int k=0; k<w[i][j].size(); ++k) {
            w[i][j][k] = d(gen)*sqrt(2.0/shape[i]);
          };
        };
      };
    };

    // destructor
    ~MLP (void) {};

    // compute output
    std::vector<double> compute_output (const std::vector<double> & in) {

      // we start with the input vector
      std::vector<double> out(in);

      // for each layer except the last
      for (int i=0; i<w.size(); ++i) {

      };

      return out;
    };

  };

  // sigmoid activation function
  double sigmoid (const double & in) {
    return 1/(1 + exp(-in));
  };

  // sigmoid activation for vector
  std::vector<double> sigmoid_vec (const std::vector<double> & in) {
    const int nnodes(in.size());
    std::vector<double> out(nnodes);
    for (int i=0; i<nnodes; ++i) {out[i] = sigmoid(out[i]);};
    return out;
  };

};


#endif
