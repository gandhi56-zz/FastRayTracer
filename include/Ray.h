#pragma once
#include "Vec3.h"

template<typename T>
class Ray {
public:
  Ray() {}
  Ray(const Point3& origin, const Vec3<T>& direction)
      : orig(origin), dir(direction)
  {}

  Point3 origin() const  { return orig; }
  Vec3<T> direction() const { return dir; }

  Point3 at(T t) const {
      return orig + t*dir;
  }

public:
  Point3 orig;
  Vec3<T> dir;
};
