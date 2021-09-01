cmake_minimum_required(VERSION 3.0.0)
project(FastRayTracer)
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_STANDARD 17 CACHE STRING "")
set(CMAKE_CXX_FLAGS "-O3")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

include_directories(include)
add_executable(raytracer src/raytracer.cc)

enable_testing()
add_test(NAME raytracer
         COMMAND raytracer)
