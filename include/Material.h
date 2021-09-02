#pragma once

#include "Hittable.h"
#include "RTWeekend.h"
#include "Ray.h"
#include "Vec3.h"

struct HitRecord;

class Material{
public:
  virtual bool scatter(const Ray<double>& inputRay, 
    const HitRecord& rec, Color& attenuation, 
    Ray<double>& scattered) const = 0;
};

class Lambertian : public Material{
public:
  Lambertian(const Color& a)
    : albedo(a) {}

  virtual bool scatter(const Ray<double>& inputRay, 
    const HitRecord& rec, Color& attenuation, 
    Ray<double>& scattered) const override {
    auto scatterDirection = rec.normal + random_unit_vector<double>();

    // catch degenerate scatter direction
    if (scatterDirection.near_zero())
      scatterDirection = rec.normal;

    scattered = Ray<double>(rec.p, scatterDirection);
    attenuation = albedo;
    return true;
  }

  Color albedo;
};

class Metal : public Material{
public:
  Metal(const Color& a, double f)
    : albedo(a), fuzz(f < 1 ? f : 1) {}
  
  virtual bool scatter(const Ray<double>& inputRay, 
    const HitRecord& rec, Color& attenuation, 
    Ray<double>& scattered) const override {
    auto reflected = reflect(unit_vector(inputRay.direction()), rec.normal);
    scattered = Ray<double>(rec.p, reflected + fuzz * random_in_unit_sphere<double>());
    attenuation = albedo;
    return (dot(scattered.direction(), rec.normal) > 0);
  }

  Color albedo;
  double fuzz;
};
