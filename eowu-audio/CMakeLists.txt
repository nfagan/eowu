cmake_minimum_required(VERSION 3.2)
project(eowu-audio VERSION 0.1 LANGUAGES CXX)

file(GLOB_RECURSE LIB_SRC 
  src/*.cpp
  src/*.h
  src/*.hpp
  src/*.hh
  include/*.hpp
)

list(APPEND LIB_SRC deps/AudioFile/AudioFile.cpp)

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

add_subdirectory(deps/rtaudio)
include_directories(deps/rtaudio)
include_directories(deps/AudioFile)

add_executable(eowu-audio-playground ${PLAYGROUND_SRC})
add_executable(eowu-audio-test ${TEST_SRC})
add_library(eowu-audio STATIC ${LIB_SRC})

target_include_directories(eowu-audio PUBLIC include/ deps/rtaudio)

set_property(TARGET eowu-audio PROPERTY CXX_STANDARD 17)
set_property(TARGET eowu-audio-playground PROPERTY CXX_STANDARD 17)
set_property(TARGET eowu-audio-test PROPERTY CXX_STANDARD 17)

target_link_libraries(eowu-audio eowu-common rtaudio_static)
target_link_libraries(eowu-audio-playground eowu-audio)
target_link_libraries(eowu-audio-test eowu-audio)


