cmake_minimum_required(VERSION 3.2)
project(eowu-common VERSION 0.1 LANGUAGES CXX)

file(GLOB_RECURSE LIB_SRC 
  src/*.cpp
  src/*.h
  src/*.hpp,
  include/*.hpp
)

add_library(eowu-common STATIC ${LIB_SRC})

set_property(TARGET eowu-common PROPERTY CXX_STANDARD 17)

target_include_directories(eowu-common PUBLIC include/)

