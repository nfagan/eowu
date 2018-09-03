cmake_minimum_required(VERSION 3.2)
project(eowu-gl VERSION 0.1 LANGUAGES CXX)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(EOWU_GL_SOURCEDIR "${CMAKE_SOURCE_DIR}/eowu-gl")
set(EOWU_GL_DEPDIR "${EOWU_GL_SOURCEDIR}/deps")

#  GLFW
add_subdirectory(deps/glfw)
include_directories(deps/glfw)

#  GLAD
include_directories(deps/glad/include)

#  GLM
include_directories(deps/glm)

#  VARIANT
include_directories(deps/variant/include)

#  STB
include_directories(deps/stb)

file(GLOB TEST_SRC
  test/*.cpp 
  test/*.h
  test/*.hpp
)

file(GLOB_RECURSE LIB_SRC 
  src/*.cpp
  src/*.h
  src/*.hpp
  include/*.hpp
)

set(GLM_INCLUDE_DIR "${EOWU_GL_DEPDIR}/glm")
set(VARIANT_INCLUDE_DIR "${EOWU_GL_DEPDIR}/variant/include")
set(EOWU_GL_INCLUDE_DIR "${EOWU_GL_SOURCEDIR}/include")

list(APPEND TEST_SRC "deps/glad/src/glad.c")
list(APPEND LIB_SRC "deps/glad/src/glad.c")

add_executable(eowu-gl-test ${TEST_SRC})
add_library(eowu-gl STATIC ${LIB_SRC})

target_include_directories(eowu-gl PUBLIC 
  ${GLM_INCLUDE_DIR} 
  ${VARIANT_INCLUDE_DIR} 
  ${EOWU_GL_INCLUDE_DIR}
)

target_include_directories(eowu-gl-test PUBLIC include/)

set_property(TARGET eowu-gl PROPERTY CXX_STANDARD 17)
set_property(TARGET eowu-gl-test PROPERTY CXX_STANDARD 17)

target_link_libraries(eowu-gl eowu-common glfw)
target_link_libraries(eowu-gl-test eowu-gl glfw)

