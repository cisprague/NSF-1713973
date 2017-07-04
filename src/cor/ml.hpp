// Christopher Iliffe Sprague
// https://cisprague.github.io
// cisprague@ac.jaxa.jp

#ifndef ml_hpp
#define ml_hpp

#include <vector>
#include <iostream>
#include <eigen3/Eigen/Dense>

using namespace std;
using namespace Eigen;

// machine learning
namespace ml {

  // multilayer perceptron
  class mlp {

    public:

      // define some usefull data types
      typedef Matrix<float, Dynamic, Dynamic> Weight;
      typedef vector<Weight> Weights;
      typedef Matrix<float, Dynamic, 1> Layer;
      typedef vector<Layer> Biases;

      vector<int> shape; // structure
      int ni; // # inputs
      int no; // # ouputs
      int nl; // # layers
      int dim; // parametre dimensions
      Weights W;
      Biases B;

      // constructor
      mlp(vector<int> shape) {
        shape = shape;
        ni = shape.front();
        no = shape.back();
        nl = shape.size();
        for (int i=0; i<nl-1; i++) {
          // we compile the parametre dimensions
          dim += shape[i+1]*(shape[i] + 1);
          // initialise randomly the weights
          W.push_back(Weight::Random(shape[i+1],shape[i]));
          // initialise randomly the biases
          B.push_back(Layer::Random(shape[i+1], 1));
        };
        cout << "Created MLP" << endl;
      };

      // destructor
      ~mlp(void) {
        cout << "Deleted MLP" << endl;
      };

      // compute activation
      Layer sigmoid(Layer input) {
        for (int i=0; i<input.size(); i++) {
          input[i] = 1 + exp(-input[i]);
          input[i] = 1/input[i];
        };
        //cout << input << endl;
        return input;
      };

      // compute output
      Layer compute_output(Layer input) {
        // for each layer except last
        for (int i=0; i<nl-1; i++) {
          input = W[i]*input + B[i];
          input = sigmoid(input);
        };
        cout << input << endl;
        return input;
      };

      // printing details
      void print(void) {
        cout << "Multilayer perceptron" << endl;
        cout << "Inputs: " << ni << endl;
        cout << "Outputs: " << no << endl;
        cout << "Layers: " << nl << endl;
        cout << "Dimensions: " << dim << endl;
        //cout << "Weights:\n " << weights[0](0,0) << endl;
      };

  };

};

#endif
