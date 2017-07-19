# Christopher Iliffe Sprague
# https://cisprague.github.io
# cisprague@ac.jaxa.jp

# version
cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 11)

# headers
include_directories(/usr/local/include/eigen3)
include_directories(../req/cspice/include) # cspice
include_directories(${Boost_INCLUDE_DIRS})
#include_directories(/usr/include/boost)
#include_directories(../req/matplotlib-cpp) # matplotlibcpp
#include_directories(${PYTHON_INCLUDE_DIR}) # python
#include_directories(/usr/local/include/pagmo/)

# programmes
add_executable(ptp ../imp/ptp.cpp)

# linkage
#link_libraries(/usr/local/lib/libpagmo.so)
#link_libraries(/usr/local/lib/)
target_link_libraries(ptp ${CMAKE_SOURCE_DIR}/../req/cspice/lib/cspice.a)
target_link_libraries(ptp ${Boost_LIBRARIES})
#target_link_libraries(fika ${PYTHON_LIBRARY})
