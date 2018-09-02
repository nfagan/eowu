cmake_minimum_required(VERSION 3.2)
project(eowu-script VERSION 0.1 LANGUAGES CXX)

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

if(APPLE)
  set(PLATFORM_DIRECTORY "maci64")
  set(LUA_LIB "liblua53.a")
else()
  set(PLATFORM_DIRECTORY "win64")
  set(LUA_LIB "lua53.lib")
endif()

add_library(lua STATIC IMPORTED)
set_target_properties(lua PROPERTIES
  IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/eowu-script/deps/lua/${PLATFORM_DIRECTORY}/${LUA_LIB}"
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/eowu-script/deps/lua/${PLATFORM_DIRECTORY}/include"
)

add_executable(eowu-script-test ${TEST_SRC})
add_library(eowu-script STATIC ${LIB_SRC})

target_include_directories(eowu-script PUBLIC include/ src/ deps/LuaBridge/Source/)
#target_include_directories(eowu-script PUBLIC include/)

target_include_directories(eowu-script-test PUBLIC src/ deps/LuaBridge/Source/)

set_property(TARGET eowu-script PROPERTY CXX_STANDARD 17)
set_property(TARGET eowu-script-test PROPERTY CXX_STANDARD 17)

target_link_libraries(eowu-script eowu-gl eowu-state lua)
target_link_libraries(eowu-script-test eowu-script)


