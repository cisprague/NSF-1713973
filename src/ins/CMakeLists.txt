# Christopher Iliffe Sprague
# https://cisprague.github.io
# cisprague@ac.jaxa.jp

# version
cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 11)

# programmes
add_executable(main ../prb/main.cpp)

find_package(PythonLibs 2.7)

# headers
include_directories(../req/cspice/include) # cspice
include_directories(../req/matplotlib-cpp) # matplotlibcpp
include_directories(/usr/local/include/yaml-cpp) # yaml
include_directories(/usr/include/boost)
include_directories(/usr/local/include/pagmo)
#include_directories(/usr/include/coin)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PYTHON_INCLUDE_DIRS})


# linkage
target_link_libraries(main /usr/local/lib/libyaml-cpp.a)
#target_link_libraries(main /usr/lib/libipopt.a)
target_link_libraries(main /usr/local/lib/libpagmo.so)
target_link_libraries(main ${CMAKE_SOURCE_DIR}/../req/cspice/lib/cspice.a)
target_link_libraries(main ${Boost_LIBRARIES})
target_link_libraries(main ${PYTHON_LIBRARIES})
