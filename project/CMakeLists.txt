#CMakeLists.txt to build project using cmake
#run $cmake CMakeLists.txt

cmake_minimum_required(VERSION 2.6)

project(pgm)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories("/usr/include/irrlicht")

file(
  GLOB_RECURSE
  source_files
  src/*.cpp
  src/*.h
)

SET(CMAKE_BUILD_TYPE Debug)
ADD_DEFINITIONS( -Wall -Wextra -O2 -g -std=c++11 -Wno-unused-parameter -I/usr/include/irrlicht)


add_executable(
  pgm
  ${source_files}
)


TARGET_LINK_LIBRARIES(pgm -lIrrlicht)


