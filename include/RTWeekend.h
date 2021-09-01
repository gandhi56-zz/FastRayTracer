#pragma once

#include <cstdlib>
#include <cmath>
#include <limits>
#include <memory>
#include <random>
#include "Ray.h"
#include "Vec3.h"

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

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
