// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp

#ifndef vecops_hpp
#define vecops_hpp
#include <vector>
#include <iostream>
#include <cmath>
#include <random>

namespace vectools {

  void display (const std::vector<double> & vec) {
    for (int i=0; i<vec.size(); i++) {std::cout << vec[i] << '\t';};
    std::cout << std::endl;
  };

  double nsum (const std::vector<double> & vec, const double & n) {
    double sum = 0;
    for (int i=0; i<vec.size(); i++) {sum += pow(vec[i], n);};
    return sum;
  };

  double norm (const std::vector<double> & vec) {
    return sqrt(nsum(vec, 2));
  };

  double normpow (const std::vector<double> & vec, const double & power) {
    return pow(norm(vec), power);
  };

  std::vector<double> section (
    const std::vector<double> & vec,
    const int & i0,
    const int & iN
  ) {
    std::vector<double> slice(iN - i0 + 1);
    for (int i=i0; i<iN+1; i++) {slice[i] = vec[i];};
    return slice;
  };

  std::vector<double> scaled (
    const std::vector<double> & vec,
    const double & scaler
  ) {
    std::vector<double> vscaled;
    for (int i=0; i<vec.size(); i++) {vscaled.push_back(vec[i]*scaler);};
    return vscaled;
  };

  std::vector<double> diff(
    const std::vector<double> & vec1,
    const std::vector<double> & vec2
  ) {
    std::vector<double> diff;
    for (int i=0; i<vec1.size(); i++) {diff.push_back(vec1[i] - vec2[i]);};
    return diff;
  };

  std::vector<double> random_unit_vec (void) {
    std::vector<double> uv(3);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> theta_(0, 2*M_PI);
    std::uniform_real_distribution<double> z_(-1, 1);
    double theta = theta_(gen);
    double z = z_(gen);
    uv[0] = sqrt(1-pow(z,2))*cos(theta);
    uv[1] = sqrt(1-pow(z,2))*sin(theta);
    uv[2] = z;
    return uv;
  };



};


#endif
