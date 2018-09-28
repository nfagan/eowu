cmake_minimum_required(VERSION 3.2)
project(eowu-common VERSION 0.1 LANGUAGES CXX)

file(GLOB_RECURSE LIB_SRC 
  src/*.cpp
  src/*.h
  src/*.hpp
  src/*.hh
  include/*.hpp
)

file(GLOB_RECURSE TEST_SRC
  test/*.cpp
  test/*.h
  test/*.hpp
)

add_library(eowu-common STATIC ${LIB_SRC})
add_executable(eowu-common-test ${TEST_SRC})

set_property(TARGET eowu-common PROPERTY CXX_STANDARD 17)
set_property(TARGET eowu-common-test PROPERTY CXX_STANDARD 17)

target_include_directories(eowu-common PUBLIC include/)
target_link_libraries(eowu-common-test eowu-common)

