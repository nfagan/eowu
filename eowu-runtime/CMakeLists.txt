cmake_minimum_required(VERSION 3.2)
project(eowu-runtime VERSION 0.1 LANGUAGES CXX)

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

set(EXEC_SRC "src/main.cpp")

add_executable(eowu-runtime-test ${TEST_SRC})
add_executable(eowu-run ${EXEC_SRC})
add_library(eowu-runtime STATIC ${LIB_SRC})

target_include_directories(eowu-runtime PUBLIC include/)
target_include_directories(eowu-runtime-test PRIVATE include/)

set_property(TARGET eowu-runtime PROPERTY CXX_STANDARD 17)
set_property(TARGET eowu-runtime-test PROPERTY CXX_STANDARD 17)
set_property(TARGET eowu-run PROPERTY CXX_STANDARD 17)

target_link_libraries(eowu-runtime eowu-script)
target_link_libraries(eowu-runtime-test eowu-runtime)
target_link_libraries(eowu-run eowu-runtime)


