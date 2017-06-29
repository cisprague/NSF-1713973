#include <iostream>
#include <eigen3/Eigen/Dense>
#include "ml.hpp"
#include "mlp.cpp"

//using namespace Eigen;
using namespace std;

int main(void) {
  // structure
  vector<int> shape = {5, 10, 2};
  // network
  ml::mlp net(shape);
  // print
  net.print();
  return 0;
}
