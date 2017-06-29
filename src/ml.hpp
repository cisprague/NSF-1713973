// Christopher Iliffe Sprague
// https://cisprague.github.io
// cisprague@ac.jaxa.jp

#ifndef ML_HPP
#define ML_HPP
#include <eigen3/Eigen/Dense>
using namespace std;
using namespace Eigen;

// machine learning
namespace ml {

  // multilayer perceptron
  class mlp {
    private:
      vector<int> shape; // structure
      int ni; // # inputs
      int no; // # ouputs
      int nl; // # layers
      int dim; // parametre dimensions
      vector<MatrixXd> weights;
    public:
      mlp(vector<int> shape); // constructor
      void print(void); // printer
  };

};

#endif
