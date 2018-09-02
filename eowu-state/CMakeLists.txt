cmake_minimum_required(VERSION 3.2)
project(eowu-state VERSION 0.1 LANGUAGES CXX)

file(GLOB TEST_SRC
  test/*.cpp 
  test/*.h
  test/*.hpp 
  include/*.hpp
)

file(GLOB_RECURSE LIB_SRC 
  src/*.cpp
  src/*.h
  src/*.hpp
  include/*.hpp
)

add_executable(eowu-state-test ${TEST_SRC})
add_library(eowu-state STATIC ${LIB_SRC})

target_include_directories(eowu-state PUBLIC include/)

set_property(TARGET eowu-state PROPERTY CXX_STANDARD 17)
set_property(TARGET eowu-state-test PROPERTY CXX_STANDARD 17)

target_link_libraries(eowu-state eowu-common)
target_link_libraries(eowu-state-test eowu-state)


