cmake_minimum_required(VERSION 3.2)
project(eowu-common VERSION 0.1 LANGUAGES CXX)

file(GLOB_RECURSE LIB_SRC 
  src/*.cpp
  src/*.h
  src/*.hpp
)

add_library(eowu-common INTERFACE)

target_include_directories(eowu-common INTERFACE include/)

