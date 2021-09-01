#pragma once

#include <cstdlib>
#include <cmath>
#include <limits>
#include <memory>
#include <random>

const double INF = std::numeric_limits<double>::infinity();
const double PI = 3.1415926535897932385;

inline double degrees_to_radians(double degrees){
  return degrees * PI / 180.0;
}

template<typename T>
inline T clamp(T x, T min, T max){
  if (x < min)  return min;
  if (x > max)  return max;
  return x;
}

template<typename T>
inline T random_t(){
  static std::uniform_real_distribution<T> distribution((T)0.0, (T)1.0);
  static std::mt19937 generator;
  return distribution(generator);
}

template<typename T>
inline double random_t(T min, T max){
  return min + (max - min) * random_t<T>();
}
