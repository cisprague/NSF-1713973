// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp
// NSF Award 1713973

#ifndef mlp_hpp
#define mlp_hpp
#include <vector>
#include <random>
#include <cmath>
#include <stdexcept>
#include <string>
#include "linalg.hpp"
#include "activation.hpp"

// machine learning
namespace ML {

  // multilayer perceptron
  struct MLP {

    // a priori
    const std::vector<int> shape;   // architecture shape
    const std::vector<double> refs; // parametres for normalisation
    const int nl;                   // number of layers
    const int ni;                   // number of inputs
    const int no;                   // number of outputs
    const int wd;                   // weight dimensions
    const int bd;                   // bias dimensions

    // a posteriori
    std::vector<std::vector<std::vector<double>>> w; // weights
    std::vector<std::vector<double>>              b; // biases

    // constructor
    MLP (const std::vector<int> & shape_, const std::vector<double> & refs_) :
      shape(shape_),
      refs(refs_),
      nl(shape.size()),
      ni(shape.front()),
      no(shape.back()),
      w(nl-1),
      b(nl-1),
      wd(weight_dim(shape)),
      bd(bias_dim(shape)) {

      // sanity
      if (ni != refs_.size()) {
        throw std::invalid_argument("Dimension of normalisation vector does not match desired dimension of input vector.");
      };

      // initialise the random number generator
      std::random_device rd;
      std::mt19937 gen(rd());

      // standard normal distribution
      std::normal_distribution<double> d(0, 1);

      // initialise columns of weight matricies and biases
      for (int i=0; i<nl-1; ++i) {
        w[i] = std::vector<std::vector<double>>(shape[i+1]);
        b[i] = std::vector<double>(shape[i+1]);

        // initialise rows of weight matricies
        // https://arxiv.org/abs/1502.01852
        for (int j=0; j<shape[i+1]; ++j) {
          w[i][j] = std::vector<double>(shape[i]);
          b[i][j] = d(gen)*sqrt(2.0/shape[i]);
          for (int k=0; k<shape[i]; ++k) {
            w[i][j][k] = d(gen)*sqrt(2.0/shape[i]);
          };
        };
      };
    };

    // alternate constructor
    MLP (
      const int & ni_,
      const int & no_,
      const std::vector<int> & hshape_,
      const std::vector<double> & refs_
    ) : MLP(full_struct(ni_, no_, hshape_), refs_) {};

    // destructor
    ~MLP (void) {};

    // compute output
    std::vector<double> operator () (std::vector<double> in) const {

      // sanity
      if (ni != in.size()) {"Input vector dimension does not match dimension of desired input dimension.";};

      // we normalise the input vector by references
      in = linalg::normref(in, refs);

      // for each layer except the last
      for (int i=0; i<nl-1; ++i) {
        in = linalg::matxvec(w[i], in); // apply weights
        in = linalg::add(in, b[i]);     // apply biases
        in = sigmoid_vec(in);           // apply activation
      };

      return in;
    };

    // assemble full structure vector
    static std::vector<int> full_struct (
      const int & ni,
      const int & no,
      const std::vector<int> & hidden_struct
    ) {
      std::vector<int> full_struct;
      full_struct.push_back(ni);
      for (int i=0; i<hidden_struct.size(); ++i) {
        full_struct.push_back(hidden_struct[i]);
      };
      full_struct.push_back(no);
      return full_struct;
    };

    // weight parameter dimensions
    static int weight_dim (const std::vector<int> & shape) {
      int wd(0);
      for (int i=0; i<shape.size()-1; ++i) {
        wd += shape[i]*shape[i+1];
      };
      return wd;
    };

    // bias dimensions
    static int bias_dim (const std::vector<int> & shape) {
      int bd(0);
      for (int i=0; i<shape.size()-1; ++i) {
        bd += shape[i+1];
      };
      return bd;
    };

    // set weights
    void set_weights (const std::vector<double> & weights) {
      int count(0);
      // for each layer
      for (int i=0; i<nl-1; ++i) {
        // for each row in matrix
        for (int j=0; j<shape[i+1]; j++) {
          // for each column in matrix
          for (int k=0; k<shape[i]; ++k) {
            w[i][j][k] = weights[count];
            ++count;
          };
        };
      };
    };

    void set_biases (const std::vector<double> & biases) {
      int count(0);
      // for each layer
      for (int i=0; i<nl-1; ++i) {
        // for each bias
        for (int j=0; j<shape[i+1]; ++j) {
          b[i][j] = biases[count];
          ++count;
        };
      };
    };

  };

};


#endif
