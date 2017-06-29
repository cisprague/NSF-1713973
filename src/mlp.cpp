#include <iostream>
#include "ml.hpp"

using namespace std;
using namespace ml;
using namespace Eigen;

mlp::mlp(vector<int> shape) {
  shape = shape;
  ni = shape.front();
  no = shape.back();
  nl = shape.size();
  // parametre dimension of weights and biases
  for (int i=0; i<nl; i++) {
    dim += shape[i+1]*(shape[i] + 1);
  };
};

void mlp::print(void) {
  cout << "Multilayer perceptron" << endl;
  cout << "Inputs: " << ni << endl;
  cout << "Outputs: " << no << endl;
  cout << "Layers: " << nl << endl;
  cout << "Dimensions: " << dim << endl;
};
