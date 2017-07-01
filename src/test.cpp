#include <iostream>
#include <eigen3/Eigen/Dense>
#include "ml.hpp"
#include "dyn.hpp"
//#include "mlp.cpp"

using namespace Eigen;
using namespace std;

// machine learning tests
namespace ml {
  void test1(void) {
    // define network structure
    vector<int> shape = {5, 10, 2};
    // instantiate network
    ml::mlp net(shape);
    // compute random output
    net.compute_output(ml::mlp::Layer::Random(5,1));
  };
};

// dynamical system tests
namespace dyn {
  void test1(void) {
    // instantiate system
    dyn::ems sc(5, 2);
    // print details
    sc.print();
  };
};

int main(void) {

  //ml::test1();
  dyn::test1();

  return 0;
};
