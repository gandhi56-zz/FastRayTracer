#include "RTWeekend.h"
#include "Color.h"
#include "HittableList.h"
#include "Ray.h"
#include "Sphere.h"
#include "Vec3.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include "Camera.h"
#include "cxxopts.hpp"

Color ray_color(const Ray<double>& r, const Hittable& world, int depth){
  HitRecord rec;

  // If we have exceeded the ray bounce limit, no more light should be gathered.
  if (depth <= 0)
    return Color(0,0,0);

  if (world.hit(r, 0.001, INF, rec)){
    Point3 target = rec.p + rec.normal + random_in_hemisphere(rec.normal);
    return 0.5 * ray_color(Ray<double>(rec.p, target - rec.p), world, depth - 1);
  }
  Vec3<double> unit_direction = unit_vector(r.direction());
  auto t = 0.5 * (unit_direction.y() + 1.0);
  return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

int main(int argc, const char** argv){
  // image
  auto aspectRatio = 16.0 / 9.0;
  int imageWidth = 1280;
  int imageHeight = 720;
  int samplesPerPixel = 50;
  int maxDepth = 30;
  std::ofstream outputFile;

  cxxopts::Options opts(argv[0], "FastRayTracer by Anshil\n");
  opts.add_options()
      ("h,help", "Print usage")
      ("o,output", "Write output to ", 
        cxxopts::value<std::string>()->default_value("render.ppm"))
      ("s,spp", "Samples per pixel", 
        cxxopts::value<int>()->default_value("50"))
      ("w,width", "Set width of the render", 
        cxxopts::value<int>()->default_value("1280"));

  auto result = opts.parse(argc, argv);

  if (result.count("help")){
    std::cout << opts.help() << std::endl;
    return 0;
  }
  if (result.count("output")){

  }
  if (result.count("spp")){
    samplesPerPixel = result["spp"].as<int>();
  }
  if (result.count("width")){
    imageWidth = result["width"].as<int>();
    imageHeight = static_cast<int>(imageWidth / aspectRatio);
  }

  outputFile.open(result["output"].as<std::string>(), std::ios::out);

  // world
  HittableList world;
  world.add(make_shared<Sphere>(Point3(0,0,-1), 0.5));
  world.add(make_shared<Sphere>(Point3(0,-100.5,-1),100));

  // camera
  Camera cam;

  // render
  std::cerr << "Resolution: " << imageWidth << " x " << imageHeight << '\n';
  std::cerr << "Samples per pixel: " << samplesPerPixel << '\n';
  std::cerr << "Aspect ratio: 16:9\n";
  outputFile << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";
  for (int j = imageHeight - 1; j >= 0; --j){
    std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
    for (int i = 0; i < imageWidth; ++i){
      Color pixelColor(0,0,0);
      for (int s = 0; s < samplesPerPixel; ++s){
        auto u = (i + random_t<double>()) / (imageWidth - 1);
        auto v = (j + random_t<double>()) / (imageHeight - 1);
        Ray<double> r = cam.getRay(u, v);
        pixelColor += ray_color(r, world, maxDepth);
      }
      write_color(outputFile, pixelColor, samplesPerPixel);
    }
  }
  return 0;
}
