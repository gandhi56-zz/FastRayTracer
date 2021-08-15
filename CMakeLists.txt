cmake_minimum_required(VERSION 3.0.0)
project(FastRayTracer)
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_STANDARD 17 CACHE STRING "")
set(CMAKE_CXX_FLAGS "-O3")

add_executable(raytracer src/raytracer.cc)