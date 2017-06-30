#include <iostream>
#include <eigen3/Eigen/Dense>
#include "ml.hpp"
//#include "mlp.cpp"

using namespace Eigen;
using namespace std;

int main(void) {
  // define structure of network
  vector<int> shape = {5, 10, 2};
  // instantiate network
  ml::mlp net(shape);
  // initialise randomly weights and biases
  //net.random_weights();
  net.compute_output(ml::mlp::Layer::Random(5,1));

  return 0;
};
