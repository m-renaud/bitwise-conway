cmake_minimum_required(VERSION 2.8)

if(CMAKE_COMPILER_IS_GNUCXX)
  add_definitions("-Wall")
  add_definitions("-Wextra")
  add_definitions("-Werror")
  add_definitions("-fno-strict-aliasing")
  add_definitions("--std=c++11")
endif()

add_executable(bitconway bitconway.cxx)
add_executable(bitconway-fast bitconway_fast.cxx)
