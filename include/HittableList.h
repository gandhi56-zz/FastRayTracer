#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "Hittable.h"

#include <memory>
#include <vector>

class HittableList : public Hittable {
public:
  HittableList() {}
  HittableList(std::shared_ptr<Hittable> object) { add(object); }

  void clear() { objects.clear(); }

  void add(std::shared_ptr<Hittable> object) { objects.push_back(object); }

  virtual bool hit(const Ray<double> &r, double tMin, double tMax,
                   HitRecord &rec) const override;

public:
  std::vector<std::shared_ptr<Hittable>> objects;
};

inline bool HittableList::hit(const Ray<double> &r, double tMin, double tMax,
                              HitRecord &rec) const {
  HitRecord tmpRec;
  bool hitAnything = false;
  auto closestSoFar = tMax;
  for (const auto &obj : objects) {
    if (obj->hit(r, tMin, closestSoFar, tmpRec)) {
      hitAnything = true;
      closestSoFar = tmpRec.t;
      rec = tmpRec;
    }
  }
  return hitAnything;
}

#endif /* HITTABLE_LIST_H */