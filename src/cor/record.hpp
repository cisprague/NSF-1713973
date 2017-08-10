// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp

#ifndef record_hpp
#define record_hpp
#include <fstream>
#include <vector>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

void write_record (const std::vector<double> & vec, const std::string & path) {

  // file container
  std::ofstream f(path);
  // write the file
  boost::archive::text_oarchive oa(f);
  oa << vec;
}

#endif
