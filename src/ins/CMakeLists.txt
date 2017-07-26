# Christopher Iliffe Sprague
# https://cisprague.github.io
# cisprague@ac.jaxa.jp

# version
cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 11)

if (UNIX AND NOT APPLE)
  execute_process(COMMAND ${CMAKE_C_COMPILER} -fuse-ld=gold -Wl,--version ERROR_QUIET OUTPUT_VARIABLE ld_version)
  if ("${ld_version}" MATCHES "GNU gold")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold -Wl,--disable-new-dtags")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fuse-ld=gold -Wl,--disable-new-dtags")
  endif()
endif()

find_package(PythonLibs 2.7)

# headers
#include_directories(/usr/local/include/eigen3)
include_directories(../req/cspice/include) # cspice
include_directories(${Boost_INCLUDE_DIRS})
include_directories(/usr/include/boost)
include_directories(../req/matplotlib-cpp) # matplotlibcpp
#include_directories(${PYTHON_INCLUDE_DIR}) # python
include_directories(${PYTHON_INCLUDE_DIRS})
#include_directories(/usr/local/include/pagmo/)

# programmes
add_executable(ptp ../imp/ptp.cpp)

# linkage
#link_libraries(/usr/local/lib/libpagmo.so)
#link_libraries(/usr/local/lib/)
target_link_libraries(ptp ${CMAKE_SOURCE_DIR}/../req/cspice/lib/cspice.a)
target_link_libraries(ptp ${Boost_LIBRARIES})
target_link_libraries(ptp ${PYTHON_LIBRARIES})
