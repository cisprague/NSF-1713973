#include <iostream>
#include "../core/controller.hpp"
#include "../core/spacecraft.hpp"
#include "../core/body.hpp"
#include "../core/spice.hpp"

int main (void) {

  // load kernels
  spice::load_kernels();

  // spacecraft
  const Spacecraft spacecraft(660, 0.0923, 3337);

  // bodies
  const std::vector<Body> bodies{Body("Earth")};

  // hidden layer shape
  const std::vector<int> hshape{10, 10, 10};

  std::cout << "Testing constructor..." << std::endl;
  Controller controller(hshape, spacecraft, bodies);
  std::cout << "Net structure with 3 bodies:\n[ ";
  for (int i=0; i<controller.shape.size(); ++i) {
    std::cout << controller.shape.at(i) << " ";
  };
  std::cout << " ]" << std::endl;

  std::cout << "Number of weights = " << controller.nw << std::endl;
  std::cout << "Number of biases = " << controller.nb << std::endl;
  std::cout << "Number of layers = " << controller.nl << std::endl;
  std::cout << "Number of inputs = " << controller.ni << std::endl;
  std::cout << "Number of outputs = " << controller.no << "\n" << std::endl;

  std::cout << "Examine weights..." << std::endl;
  for (int i=0; i<controller.nl - 1; ++i) {
    for (int j=0; j<controller.shape.at(i+1); ++j) {
      for (int k=0; k<controller.shape.at(i); ++k) {
        std::cout << controller.weights.at(i).at(j).at(k) << " ";
      };
      std::cout << "\n";
    }
    std::cout << std::endl;
  };

  std::cout << "Examine biases..." << std::endl;
  for (int i=0; i<controller.nl - 1; ++i) {
    for (int j=0; j<controller.shape.at(i+1); ++j) {
      std::cout << controller.biases.at(i).at(j) << " ";
    }
    std::cout << "\n" << std::endl;
  };

  // times
  const double t0(spice::mjd2000("11/12/2004 00:00:00.000"));

  // states
  std::vector<double> x0(spice::state(t0, "1"));
  x0.push_back(spacecraft.mass);

  std::cout << "Testing controller input normaliser..." << std::endl;
  std::cout << "Inputs:\n";
  std::cout << "[ ";
  for (int i=0; i<7; ++i) {
    std::cout << x0.at(i) << " ";
  };
  std::cout << " ]" << std::endl;
  std::cout << "Time: " << t0 << std::endl;
  const std::vector<double> out(controller.normalise(x0, t0));
  std::cout << "Normalised:\n[ ";
  for (int i=0; i<out.size(); ++i) {
    std::cout << out.at(i) << " ";
  };
  std::cout << " ]\n" << std::endl;

  std::cout << "Testing control ouput..." << std::endl;
  const std::vector<double> u(controller(x0, t0));
  std::cout << "Control:\n";
  std::cout << "[ ";
  for (int i=0; i<u.size(); ++i) {
    std::cout << u.at(i) << " ";
  };
  std::cout << " ]\n" << std::endl;

  std::cout << "Testing weight setter..." << std::endl;
  std::cout << "with a vector of zeros" << std::endl;
  const std::vector<double> weights(controller.nw, 0);
  controller.set_weights(weights);

  std::cout << "Examine weights..." << std::endl;
  for (int i=0; i<controller.nl - 1; ++i) {
    for (int j=0; j<controller.shape.at(i+1); ++j) {
      for (int k=0; k<controller.shape.at(i); ++k) {
        std::cout << controller.weights.at(i).at(j).at(k) << " ";
      };
      std::cout << "\n";
    }
    std::cout << std::endl;
  };

  std::cout << "Testing bias setter..." << std::endl;
  std::cout << "with a vector of zeros" << std::endl;
  const std::vector<double> biases(controller.nb, 0);
  controller.set_biases(biases);

  std::cout << "Examine biases..." << std::endl;
  for (int i=0; i<controller.nl - 1; ++i) {
    for (int j=0; j<controller.shape.at(i+1); ++j) {
      std::cout << controller.biases.at(i).at(j) << " ";
    }
    std::cout << "\n" << std::endl;
  };

  std::cout << "Computing output again..." << std::endl;
  const std::vector<double> u1(controller(x0, t0));
  std::cout << "Control:\n";
  std::cout << "[ ";
  for (int i=0; i<u1.size(); ++i) {
    std::cout << u1.at(i) << " ";
  };
  std::cout << " ]\n" << std::endl;


  return 0;
};
