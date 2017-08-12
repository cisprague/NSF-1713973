# version
cmake_minimum_required(VERSION 3.7)

# options
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Og")
set(CMAKE_CXX_FLAGS "-pthread")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../bin/tst)

# includes
include_directories(${CMAKE_SOURCE_DIR}/../req/cspice/include)
include_directories(/usr/include/boost)

# tests
add_executable(constants ${CMAKE_SOURCE_DIR}/constants.cpp)
add_executable(spacecraft ${CMAKE_SOURCE_DIR}/spacecraft.cpp)
add_executable(spice ${CMAKE_SOURCE_DIR}/spice.cpp)
add_executable(body ${CMAKE_SOURCE_DIR}/body.cpp)
add_executable(phase ${CMAKE_SOURCE_DIR}/phase.cpp)
add_executable(controller ${CMAKE_SOURCE_DIR}/controller.cpp)
add_executable(dynamics ${CMAKE_SOURCE_DIR}/dynamics.cpp)
add_executable(propagator ${CMAKE_SOURCE_DIR}/propagator.cpp)

# linking
target_link_libraries(spice ${CMAKE_SOURCE_DIR}/../req/cspice/lib/cspice.a)
target_link_libraries(body ${CMAKE_SOURCE_DIR}/../req/cspice/lib/cspice.a)
target_link_libraries(phase ${CMAKE_SOURCE_DIR}/../req/cspice/lib/cspice.a)
target_link_libraries(controller ${CMAKE_SOURCE_DIR}/../req/cspice/lib/cspice.a)
target_link_libraries(dynamics ${CMAKE_SOURCE_DIR}/../req/cspice/lib/cspice.a)
target_link_libraries(propagator ${CMAKE_SOURCE_DIR}/../req/cspice/lib/cspice.a)
