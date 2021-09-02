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

class Dielectric : public Material {
public:
  Dielectric(double index_of_refraction) : ir(index_of_refraction) {}

  virtual bool scatter(
      const Ray<double>& r_in, const HitRecord& rec, Color& attenuation, Ray<double>& scattered
  ) const override {
    attenuation = Color(1.0, 1.0, 1.0);
    double refraction_ratio = rec.frontFace ? (1.0/ir) : ir;

    auto unit_direction = unit_vector(r_in.direction());
    double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

    bool cannot_refract = refraction_ratio * sin_theta > 1.0;
    Vec3<double> direction;

    if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_dbl())
        direction = reflect(unit_direction, rec.normal);
    else
        direction = refract(unit_direction, rec.normal, refraction_ratio);

    scattered = Ray(rec.p, direction);
    return true;
  }

private:
  static double reflectance(double cosine, double refIdx){
    auto r0 = (1 - refIdx) / (1 + refIdx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
  }

public:
  double ir; // Index of Refraction
};
