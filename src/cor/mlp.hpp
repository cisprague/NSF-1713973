// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp
// NSF Award 1713973

#ifndef mlp_hpp
#define mlp_hpp
#include <vector>

// machine learning
namespace ML {

  // multilayer perceptron
  struct MLP {

    // a priori
    const std::vector<int> sh; // shape of hidden layers
    const int              nh; // number of hidden layers
    const int              ni; // number of inputs
    const int              no; // number of outputs

    // a posteriori
    std::vector<std::vector<std::vector<double>>> w; // weights
    std::vector<std::vector<double>>              b; // biases

    // constructor
    MLP (const std::vector<int> & sh_, const int & ni_, const int & no_) :
      sh(sh_),
      ni(ni_),
      no(no_),
      nh(sh_.size()),
      w(nh + 1),
      b(nh + 1) {};

    // destructor
    ~MLP (void) {};



  };

};

#endif
