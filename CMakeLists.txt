cmake_minimum_required(VERSION 3.7.0)
project(FastRayTracer)
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_STANDARD 17 CACHE STRING "")
set(CMAKE_CXX_FLAGS "-O3")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

find_package(OpenCL REQUIRED)
include_directories(include)
add_executable(raytracer src/raytracer.cc src/Image.cc)
target_link_libraries(raytracer OpenCL::OpenCL)

enable_testing()
add_test(NAME raytracer960
         COMMAND raytracer --width=960)
add_test(NAME raytracer1280
         COMMAND raytracer --width=1280)
add_test(NAME raytracer1920
         COMMAND raytracer --width=1920)
add_test(NAME raytracer3840
         COMMAND raytracer --width=3840)
