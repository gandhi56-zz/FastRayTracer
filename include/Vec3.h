#pragma once

#include <cmath>
#include <iostream>
#include <math.h>
#include <random>
#include "RTWeekend.h"

using std::sqrt;

/// Return a random real in [0,1)
inline double random_dbl(){
  static std::uniform_real_distribution<double> distribution(0.0, 1.0);
  static std::mt19937 generator;
  return distribution(generator);
}

/// Return a random real in [min, max)
inline double random_dbl(double min, double max){
  return min + (max - min) * random_dbl();
}

template<typename T>
class Vec3{
public:
  Vec3()  : e{0,0,0} {}
  Vec3(T e0, T e1, T e2) : e{e0, e1, e2} {}

  T x() const { return e[0]; }
  T y() const { return e[1]; }
  T z() const { return e[2]; }

  Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
  T operator[](int i) const { return e[i]; }
  T& operator[](int i) { return e[i]; }

  Vec3& operator+=(const Vec3 &v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
  }

  Vec3& operator*=(const T t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
  }

  Vec3& operator/=(const T t) {
    return *this *= 1/t;
  }

  T length() const {
    return sqrt(length_squared());
  }

  T length_squared() const {
    return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
  }

  inline static Vec3<T> rand(){
    return Vec3<T>(random_t<T>(), random_t<T>(), random_t<T>());
  }

  inline static Vec3<T> rand(T min, T max){
    return Vec3<T>(random_t(min, max), random_t(min, max), random_t(min, max));
  }

  /// returns true if the vector is close to zero in all dimensions
  bool near_zero() const {
    const auto eps = 1e-8;
    return (fabs(e[0]) < eps) && (fabs(e[1]) < eps) && (fabs(e[2]) < eps);
  }

  T e[3];
};

using Point3 = Vec3<double>;
using Color = Vec3<double>;

// Vec3 Utility Functions
template<typename T>
inline std::ostream& operator<<(std::ostream &out, const Vec3<T> &v) {
  return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}
template<typename T>
inline Vec3<T> operator+(const Vec3<T> &u, const Vec3<T> &v) {
  return Vec3<T>(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}
template<typename T>
inline Vec3<T> operator-(const Vec3<T> &u, const Vec3<T> &v) {
  return Vec3<T>(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}
template<typename T>
inline Vec3<T> operator*(const Vec3<T> &u, const Vec3<T> &v) {
  return Vec3<T>(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}
template<typename T>
inline Vec3<T> operator*(T t, const Vec3<T> &v) {
  return Vec3<T>(t*v.e[0], t*v.e[1], t*v.e[2]);
}
template<typename T>
inline Vec3<T> operator*(const Vec3<T> &v, T t) {
  return t * v;
}

template<typename T>
inline Vec3<T> operator/(Vec3<T> v, T t) {
  return (1/t) * v;
}

template<typename T>
inline T dot(const Vec3<T> &u, const Vec3<T> &v) {
  return u.e[0] * v.e[0]
        + u.e[1] * v.e[1]
        + u.e[2] * v.e[2];
}

template<typename T>
inline Vec3<T> cross(const Vec3<T> &u, const Vec3<T> &v) {
  return Vec3<T>(u.e[1] * v.e[2] - u.e[2] * v.e[1],
              u.e[2] * v.e[0] - u.e[0] * v.e[2],
              u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

template<typename T>
inline Vec3<T> unit_vector(Vec3<T> v) {
  return v / v.length();
}

/// Lambertian diffuse method
template<typename T>
inline Vec3<T> random_in_unit_sphere(){
  while (1){
    auto p = Vec3<T>::rand(-1., 1.);
    if (p.length_squared() >= 1)  continue;
    return p;
  }
}

template<typename T>
Vec3<T> random_unit_vector(){
  return unit_vector(random_in_unit_sphere<T>());
}

/// Uniform scatter method
template<typename T>
Vec3<T> random_in_hemisphere(const Vec3<T>& normal){
  Vec3<T> inUnitSphere = random_in_unit_sphere<T>();
  if (dot(inUnitSphere, normal) < 0.0)
    return inUnitSphere;
  return -inUnitSphere;
}

template<typename T>
Vec3<T> reflect(const Vec3<T>& v, const Vec3<T>& n){
  return v - 2 * dot(v, n) * n;
}

/// Return the direction of the refracted vector as a vector
template<typename T>
Vec3<T> refract(const Vec3<T>& uv, const Vec3<T>& n, double etai_over_etat) {
    auto cosTheta = fmin(dot(-uv, n), 1.0);
    Vec3<T> outputRayPerp =  etai_over_etat * (uv + cosTheta*n);
    Vec3<T> outputRayParallel = -sqrt(fabs(1.0 - outputRayPerp.length_squared())) * n;
    return outputRayPerp + outputRayParallel;
}

template<typename T>
Vec3<T> random_in_unit_disk(){
  while (1){
    auto p = Vec3(random_dbl(-1., 1.), random_dbl(-1., 1.), 0.0);
    if (p.length_squared() >= 1)  continue;
    return p;
  }
}