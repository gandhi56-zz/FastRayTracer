#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cstdlib>
#include <cmath>
#include <limits>
#include <memory>
#include <random>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

const double INF = std::numeric_limits<double>::infinity();
const double PI = 3.1415926535897932385;

inline double degrees_to_radians(double degrees){
  return degrees * PI / 180.0;
}

#include "Ray.h"
#include "Vec3.h"

/// Return a random real in [0,1)
template<typename T>
inline T random(){
  static std::uniform_real_distribution<T> distribution((T)0, (T)1);
  static std::mt19937 generator;
  return distribution(generator);
}

/// Return a random real in [min, max)
template<typename T>
inline T random(T min, T max){
  return min + (max - min) * random<T>();
}

template<typename T>
inline T clamp(T x, T min, T max){
  if (x < min)  return min;
  if (x > max)  return max;
  return x;
}

#endif /* RTWEEKEND_H */