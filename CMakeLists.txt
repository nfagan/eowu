cmake_minimum_required(VERSION 3.2)
project(eowu VERSION 0.1 LANGUAGES CXX)

set(EOWU_CXX_STANDARD, 17)

add_subdirectory(eowu-common)
add_subdirectory(eowu-state)
add_subdirectory(eowu-gl)
add_subdirectory(eowu-script)
add_subdirectory(eowu-runtime)