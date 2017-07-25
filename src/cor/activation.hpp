// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp

#ifndef activation_hpp
#define activation_hpp
#include <vector>

namespace ML {

  // sigmoid activation function
  double sigmoid (const double & in) {
    return 1/(1 + exp(-in));
  };

  // sigmoid activation for vector
  std::vector<double> sigmoid_vec (const std::vector<double> & in) {
    const int nnodes(in.size());
    std::vector<double> out(nnodes);
    for (int i=0; i<nnodes; ++i) {out[i] = sigmoid(in[i]);};
    return out;
  };
};

#endif
