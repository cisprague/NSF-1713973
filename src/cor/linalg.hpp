// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp
// NSF Award 1713973

#ifndef vecops_hpp
#define vecops_hpp
#include <vector>
#include <iostream>
#include <cmath>
#include <random>

namespace linalg {

  template <typename T>
  void display_vec (const std::vector<T> & vec) {
    for (int i=0; i<vec.size(); ++i) {std::cout << vec[i] << '\t';};
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

  std::vector<double> add (
    const std::vector<double> & vec1,
    const std::vector<double> & vec2
  ) {
    const int nelem(vec1.size());
    std::vector<double> res(nelem);
    for (int i=0; i<nelem; ++i) {res[i] = vec1[i] + vec2[i];};
    return res;
  };

  std::vector<double> matxvec (
    const std::vector<std::vector<double>> & mat,
    const std::vector<double> & vec
  ) {
    // matrix and vector dimensions
    const int N(mat.size()); // [N x M]
    const int M(vec.size()); // [M x 1]
    // allocate resulting vector
    std::vector<double> prod(N); // [N x 1]
    // compute products
    for (int i=0; i<N; ++i) {
      for (int k=0; k<M; ++k) {
        prod[i] += mat[i][k]*vec[k];
      };
    };
    return prod;
  };

  // normalise vector by reference parameters
  std::vector<double> normref (
    std::vector<double> vec,
    const std::vector<double> & ref
  ) {
    for (int i=0; i<vec.size(); ++i) {vec.at(i) /= ref.at(i);};
    return vec;
  };

  std::vector<double> unit_vector (const double & theta, const double & z) {
    std::vector<double> uv(3);
    uv[0] = sqrt(1-pow(z,2))*cos(theta);
    uv[1] = sqrt(1-pow(z,2))*sin(theta);
    uv[2] = z;
    return uv;
  };

  std::vector<double> scaled_unit_vector(
    const double & mag,
    const double & theta,
    const double & z
  ) {
    return scaled(unit_vector(theta, z), mag);
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

  std::vector<double> random_scaled_unit_vec (const double & scaler) {
    return scaled(random_unit_vec(), scaler);
  };

  std::vector<double> normalise (std::vector<double> vec) {
    // we compute the norm
    const double normal(norm(vec));
    // we divide by the norm
    for (int i=0; i<vec.size(); ++i) {vec[i] /= normal;};
    return vec;
  };

  std::vector<double> cross_prod_vec (
    const std::vector<double> & b,
    const std::vector<double> & c
  ) {
    // create the product vector
    std::vector<double> a(3);
    // compute the products
    a[0] = b[1]*c[2] - b[2]*c[1];
    a[1] = b[2]*c[0] - b[0]*c[2];
    a[2] = b[0]*c[1] - b[1]*c[0];
    return a;
  };



};


#endif
