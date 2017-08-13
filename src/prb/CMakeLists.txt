# version
cmake_minimum_required(VERSION 3.7)

# options
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Og")
set(CMAKE_CXX_FLAGS "-pthread")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../bin/prb)

# includes
include_directories(${CMAKE_SOURCE_DIR}/../req/cspice/include)
include_directories(/usr/include/boost)
include_directories(/usr/local/include/pagmo)
include_directories(/usr/local/include/yaml-cpp)
include_directories(/usr/include/eigen3)

# problems programmes
add_executable(ptp ${CMAKE_SOURCE_DIR}/ptp/ptp.cpp)

# linking
target_link_libraries(ptp ${CMAKE_SOURCE_DIR}/../req/cspice/lib/cspice.a)
target_link_libraries(ptp ${Boost_LIBRARIES})
target_link_libraries(ptp /usr/local/lib/libpagmo.so)
target_link_libraries(ptp /usr/local/lib/libyaml-cpp.a)
