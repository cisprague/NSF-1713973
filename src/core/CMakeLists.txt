# Christopher Iliffe Sprague
# https://cisprague.github.io
# cisprague@ac.jaxa.jp

# version
cmake_minimum_required(VERSION 3.7)

# options
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Og")
set(CMAKE_CXX_FLAGS "-pthread")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

# tests
add_executable(constants ../tests/constants.cpp)


find_package(PythonLibs 2.7)

# headers
include_directories(../req/cspice/include) # cspice
include_directories(../req/matplotlib-cpp) # matplotlibcpp
include_directories(/usr/local/include/yaml-cpp) # yaml
include_directories(/usr/include/boost)
include_directories(/usr/local/include/pagmo)
include_directories(/usr/include/eigen3)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PYTHON_INCLUDE_DIRS})


# linkage
target_link_libraries(ptp /usr/local/lib/libyaml-cpp.a)
target_link_libraries(ptp /usr/local/lib/libpagmo.so)
target_link_libraries(ptp ${CMAKE_SOURCE_DIR}/../req/cspice/lib/cspice.a)
target_link_libraries(ptp ${Boost_LIBRARIES})
target_link_libraries(ptp ${PYTHON_LIBRARIES})
