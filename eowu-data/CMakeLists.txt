cmake_minimum_required(VERSION 3.2)
project(eowu-data VERSION 0.1 LANGUAGES CXX)

file(GLOB_RECURSE LIB_SRC 
  src/*.cpp
  src/*.h
  src/*.hpp
  src/*.hh
  include/*.hpp
)

file(GLOB TEST_SRC
  test/*.cpp 
  test/*.h
  test/*.hpp 
  include/*.hpp
)

file(GLOB_RECURSE PLAYGROUND_SRC 
  playground/*.cpp
  playground/*.h
  playground/*.hpp
  include/*.hpp
)

add_executable(eowu-data-playground ${PLAYGROUND_SRC})
add_executable(eowu-data-test ${TEST_SRC})
add_library(eowu-data STATIC ${LIB_SRC})

target_include_directories(eowu-data PUBLIC include/)

set_property(TARGET eowu-data PROPERTY CXX_STANDARD 17)
set_property(TARGET eowu-data-playground PROPERTY CXX_STANDARD 17)
set_property(TARGET eowu-data-test PROPERTY CXX_STANDARD 17)

target_link_libraries(eowu-data eowu-common)
target_link_libraries(eowu-data-playground eowu-data)
target_link_libraries(eowu-data-test eowu-data)


