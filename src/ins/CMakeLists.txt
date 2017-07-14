# Christopher Iliffe Sprague
# https://cisprague.github.io
# cisprague@ac.jaxa.jp

# version
cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 11)

# headers
include_directories(/usr/local/include/eigen3)
include_directories(../req/cspice/include) # cspice
#include_directories(../req/matplotlib-cpp) # matplotlibcpp
#include_directories(${PYTHON_INCLUDE_DIR}) # python
#include_directories(/usr/local/include/pagmo/)

# programmes
#add_executable(tst ../tst/tst.cpp)
add_executable(prb ../tst/prb_tst.cpp)

# linkage
#link_libraries(/usr/local/lib/libpagmo.so)
#link_libraries(/usr/local/lib/)
target_link_libraries(prb ${CMAKE_SOURCE_DIR}/../req/cspice/lib/cspice.a)
#target_link_libraries(fika ${PYTHON_LIBRARY})
