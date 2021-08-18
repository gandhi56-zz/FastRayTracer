#include "Vec3.h"
#include "Color.h"
#include "Ray.h"
#include <iostream>

// Determine if ray r intersects a sphere, return
// the parameter t if so, otherwise return -1.0
double hit_sphere(const Point3& center, double radius, const Ray<double>& r){
  Vec3<double> oc = r.origin() - center;
  auto a = r.direction().length_squared();
  auto half_b = dot(oc, r.direction());
  auto c = oc.length_squared() - radius * radius;
  auto discriminant = half_b * half_b - a * c;
  return (discriminant < 0 
    ? -1.0
    : -half_b - sqrt(discriminant) / a;
}

Color ray_color(const Ray<double>& r){
  auto t = hit_sphere(Point3(0,0,-1), 0.5, r);
  if (t > 0.0){
    Vec3<double> N = unit_vector(r.at(t) - Vec3<double>(0,0,-1));
    return 0.5 * Color(N.x() + 1, N.y() + 1, N.z() + 1);
  }
  Vec3<double> unit_direction = unit_vector(r.direction());
  t = 0.5 * (unit_direction.y() + 1.0);
  return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

int main(){
  // image
  const auto aspect_ratio = 16.0 / 9.0;
  const int image_width = 1280;
  const int image_height = static_cast<int>(image_width / aspect_ratio);

  // camera
  auto viewport_height = 2.0;
  auto viewport_width = aspect_ratio * viewport_height;
  auto focal_length = 1.0;
  auto origin = Point3(0,0,0);
  auto horizontal = Vec3<double>(viewport_width, 0, 0);
  auto vertical = Vec3<double>(0, viewport_height, 0);
  auto lower_left_corner = origin - horizontal / 2.0 - vertical / 2.0 
    - Vec3<double>(0.,0.,focal_length);

  // render
  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
  for (int j = image_height - 1; j >= 0; --j){
    std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
    for (int i = 0; i < image_width; ++i){
      auto u = double(i) / (image_width - 1);
      auto v = double(j) / (image_height - 1);
      Ray<double> r(origin, lower_left_corner + u * horizontal + v * 
        vertical - origin);
      Color pixel_color = ray_color(r);
      write_color(std::cout, pixel_color);
    }
  }
  return 0;
}
