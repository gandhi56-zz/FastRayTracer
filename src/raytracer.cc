#include "RTWeekend.h"
#include "Color.h"
#include "HittableList.h"
#include "Ray.h"
#include "Sphere.h"
#include "Vec3.h"
#include <cstdlib>
#include <iostream>
#include "Camera.h"

Color ray_color(const Ray<double>& r, const Hittable& world, int depth){
  HitRecord rec;

  // If we have exceeded the ray bounce limit, no more light should be gathered.
  if (depth <= 0)
    return Color(0,0,0);

  if (world.hit(r, 0, INF, rec)){
    Point3 target = rec.p + rec.normal + random_in_unit_sphere<double>();
    return 0.5 * ray_color(Ray<double>(rec.p, target - rec.p), world, depth - 1);
  }
  Vec3<double> unit_direction = unit_vector(r.direction());
  auto t = 0.5 * (unit_direction.y() + 1.0);
  return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

int main(){
  // image
  const auto aspect_ratio = 16.0 / 9.0;
  const int image_width = 480;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const int samples_per_pixel = 50;
  const int max_depth = 30;

  // world
  HittableList world;
  world.add(make_shared<Sphere>(Point3(0,0,-1), 0.5));
  world.add(make_shared<Sphere>(Point3(0,-100.5,-1),100));

  // camera
  Camera cam;

  // render
  std::cerr << "Rendering a " << image_width << " x " << image_height << " image\n";
  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
  for (int j = image_height - 1; j >= 0; --j){
    std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
    for (int i = 0; i < image_width; ++i){
      Color pixelColor(0,0,0);
      for (int s = 0; s < samples_per_pixel; ++s){
        auto u = (i + random_t<double>()) / (image_width - 1);
        auto v = (j + random_t<double>()) / (image_height - 1);
        Ray<double> r = cam.getRay(u, v);
        pixelColor += ray_color(r, world, max_depth);
      }
      write_color(std::cout, pixelColor, samples_per_pixel);
    }
  }
  return 0;
}
