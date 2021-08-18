#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

struct HitRecord {
    Point3 p;
    Vec3<double> normal;
    double t;
};

class Hittable {
    public:
        virtual bool hit(const Ray<double>& r, double t_min, 
          double t_max, HitRecord& rec) const = 0;
};

#endif