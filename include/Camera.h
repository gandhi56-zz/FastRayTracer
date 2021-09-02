#ifndef CAMERA_H
#define CAMERA_H

#include "RTWeekend.h"
#include "Ray.h"
#include "Vec3.h"

class Camera{
public:
  Camera(Point3 lookFrom, Point3 lookAt, Vec3<double> vup, double vfov, double aspectRatio,
    double aperture, double focusDist){
    auto theta = degrees_to_radians(vfov);
    auto h = tan(theta / 2.0);
    auto viewportHeight = 2.0 * h;
    auto viewportWidth = aspectRatio * viewportHeight;

    auto focalLength = 1.0;

    w = unit_vector(lookFrom - lookAt);
    u = unit_vector(cross(vup, w));
    v = cross(w, u);

    origin = lookFrom;
    horizontal = focusDist * viewportWidth * u;
    vertical = focusDist * viewportHeight * v;
    lowerLeftCorner = origin - horizontal/2. - vertical/2. - focusDist * w;
    lensRadius = aperture / 2.;
  }

  Ray<double> getRay(double s, double t) const {
    auto rd = lensRadius * random_in_unit_disk<double>();
    auto offset = u * rd.x() + v * rd.y();
    return Ray(origin + offset, lowerLeftCorner + s*horizontal + t*vertical - origin - offset);
  }

private:
  Point3 origin;
  Point3 lowerLeftCorner;
  Vec3<double> horizontal;
  Vec3<double> vertical;
  Vec3<double> u, v, w;
  double lensRadius;
};

#endif /* CAMERA_H */