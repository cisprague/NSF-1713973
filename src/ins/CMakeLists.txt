# Christopher Iliffe Sprague
# https://cisprague.github.io
# cisprague@ac.jaxa.jp

# version
cmake_minimum_required(VERSION 3.5)

# project name
project(Fika)

# headers
include_directories(../cor)
include_directories(../req/cspice/include)

# programmes
add_executable(fika ../tst/spc_tst.cpp)

# linkage
target_link_libraries(fika ${CMAKE_SOURCE_DIR}/../req/cspice/lib/cspice.a)
