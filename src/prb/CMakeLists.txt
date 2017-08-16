# version
cmake_minimum_required(VERSION 3.7)

# options
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-O3")
set(CMAKE_CXX_FLAGS "-pthread")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../../usr)

# find libraries
find_package(PythonLibs 2.7)

# includes
include_directories(${CMAKE_SOURCE_DIR}/../req/cspice/include)
include_directories(${CMAKE_SOURCE_DIR}/../req/matplotlib-cpp)
include_directories(/usr/include/boost)
include_directories(/usr/local/include/pagmo)
include_directories(/usr/local/include/yaml-cpp)
include_directories(/usr/include/eigen3)
include_directories(${PYTHON_INCLUDE_DIRS})

# problems programmes
add_executable(ptp ${CMAKE_SOURCE_DIR}/ptp.cpp)
add_executable(ptp_grad ${CMAKE_SOURCE_DIR}/ptp_grad.cpp)

# linking
target_link_libraries(ptp ${CMAKE_SOURCE_DIR}/../req/cspice/lib/cspice.a)
target_link_libraries(ptp ${Boost_LIBRARIES})
target_link_libraries(ptp /usr/local/lib/libpagmo.so)
target_link_libraries(ptp /usr/local/lib/libyaml-cpp.a)
target_link_libraries(ptp ${PYTHON_LIBRARIES})

target_link_libraries(ptp_grad ${CMAKE_SOURCE_DIR}/../req/cspice/lib/cspice.a)
target_link_libraries(ptp_grad ${Boost_LIBRARIES})
target_link_libraries(ptp_grad /usr/local/lib/libpagmo.so)
target_link_libraries(ptp_grad /usr/local/lib/libyaml-cpp.a)
target_link_libraries(ptp_grad ${PYTHON_LIBRARIES})
target_link_libraries(ptp_grad /usr/local/lib/libnlopt.a)
