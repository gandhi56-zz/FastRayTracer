#ifndef SPHERE_H
#define SPHERE_H

#include "Hittable.h"
#include "Vec3.h"

class Sphere : public Hittable {
public:
  Sphere() {}
  Sphere(Point3 cen, double r, std::shared_ptr<Material> m)
      : center(cen), radius(r), matPtr(m){};

  virtual bool hit(const Ray<double> &r, double tMin, double tMax,
                   HitRecord &rec) const override;

  Point3 center;
  double radius;
  std::shared_ptr<Material> matPtr;
};

inline bool Sphere::hit(const Ray<double> &r, double tMin, double tMax,
                        HitRecord &rec) const {
  Vec3<double> oc = r.origin() - center;
  auto a = r.direction().length_squared();
  auto half_b = dot(oc, r.direction());
  auto c = oc.length_squared() - radius * radius;

  auto discriminant = half_b * half_b - a * c;
  if (discriminant < 0)
    return false;
  auto sqrtd = sqrt(discriminant);

  // Find the nearest root that lies in the acceptable range.
  auto root = (-half_b - sqrtd) / a;
  if (root < tMin || tMax < root) {
    root = (-half_b + sqrtd) / a;
    if (root < tMin || tMax < root)
      return false;
  }

  rec.t = root;
  rec.p = r.at(rec.t);
  Vec3<double> outwardNormal = (rec.p - center) / radius;
  rec.set_face_normal(r, outwardNormal);
  rec.matPtr = matPtr;

  return true;
}

#endif
