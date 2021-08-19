#ifndef CAMERA_H
#define CAMERA_H

#include "RTWeekend.h"

class Camera{
public:
  Camera(){
    auto aspectRatio = 16.0 / 9.0;
    auto viewportHeight = 2.0;
    auto viewportWidth = aspectRatio * viewportHeight;
    auto focalLength = 1.0;

    origin = Point3(0,0,0);
    horizontal = Vec3<double>(viewportWidth, 0.0, 0.0);
    vertical = Vec3<double>(0.0, viewportHeight, 0.0);
    lowerLeftCorner = origin - horizontal / 2.0 - 
      vertical / 2.0 - Vec3<double>(0,0,focalLength);
  }

  Ray<double> getRay(double u, double v) const {
    return Ray<double>(origin, lowerLeftCorner + 
      u*horizontal + v*vertical - origin);
  }

private:
  Point3 origin;
  Point3 lowerLeftCorner;
  Vec3<double> horizontal;
  Vec3<double> vertical;
};

#endif /* CAMERA_H */