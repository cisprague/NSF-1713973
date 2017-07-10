# Christopher Iliffe Sprague
# https://cisprague.github.io
# cisprague@ac.jaxa.jp

# version
cmake_minimum_required(VERSION 3.7)

# headers
include_directories(../req/cspice/include) # cspice
include_directories(../req/matplotlib-cpp) # matplotlibcpp
include_directories(${PYTHON_INCLUDE_DIR}) # python

# programmes
add_executable(fika ../tst/msn_tst.cpp)

# linkage
target_link_libraries(fika ${CMAKE_SOURCE_DIR}/../req/cspice/lib/cspice.a)
target_link_libraries(fika ${PYTHON_LIBRARY})
