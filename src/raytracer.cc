#include "Camera.h"
#include "Color.h"
#include "HittableList.h"
#include "Material.h"
#include "RTWeekend.h"
#include "Ray.h"
#include "Sphere.h"
#include "Vec3.h"
#include "cxxopts.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>

#include <CL/cl.h>

Color ray_color(const Ray<double>& r, const Hittable& world, int depth){
  HitRecord rec;

  // If we have exceeded the ray bounce limit, no more light should be gathered.
  if (depth <= 0)
    return Color(0, 0, 0);

  if (world.hit(r, 0.001, INF, rec)) {
    Ray<double> scattered;
    Color attenuation;
    if (rec.matPtr->scatter(r, rec, attenuation, scattered))
      return attenuation * ray_color(scattered, world, depth - 1);
    return Color(0, 0, 0);
  }
  Vec3<double> unit_direction = unit_vector(r.direction());
  auto t = 0.5 * (unit_direction.y() + 1.0);
  return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

HittableList random_scene() {
  HittableList world;

  auto ground_material = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
  world.add(
      std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      auto choose_mat = random_dbl();
      Point3 center(a + 0.9 * random_dbl(), 0.2, b + 0.9 * random_dbl());

      if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
        std::shared_ptr<Material> sphere_material;

        if (choose_mat < 0.8) {
          // diffuse
          auto albedo = Color::rand() * Color::rand();
          sphere_material = std::make_shared<Lambertian>(albedo);
          world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
        } else if (choose_mat < 0.95) {
          // metal
          auto albedo = Color::rand(0.5, 1);
          auto fuzz = random_dbl(0, 0.5);
          sphere_material = std::make_shared<Metal>(albedo, fuzz);
          world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
        } else {
          // glass
          sphere_material = std::make_shared<Dielectric>(1.5);
          world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
        }
      }
    }
  }

  auto material1 = std::make_shared<Dielectric>(1.5);
  world.add(std::make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

  auto material2 = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
  world.add(std::make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

  auto material3 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
  world.add(std::make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

  return world;
}

int main(int argc, const char** argv){
  // Get platform and device information
  cl_platform_id platform_id = NULL;
  cl_device_id device_id = NULL;
  cl_uint ret_num_devices;
  cl_uint ret_num_platforms;
  cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
  ret = clGetDeviceIDs( platform_id, CL_DEVICE_TYPE_DEFAULT, 1,
          &device_id, &ret_num_devices);

  // image
  auto aspectRatio = 3. / 2.;
  int imageWidth = 960;
  int imageHeight = 640;
  int samplesPerPixel = 30;
  int maxDepth = 50;
  std::ofstream outputFile;

  // add commandline options
  cxxopts::Options opts(argv[0], "FastRayTracer by Anshil\n");
  opts.add_options()("h,help", "Print usage")(
      "o,output", "Write output to ",
      cxxopts::value<std::string>()->default_value("render.ppm"))(
      "s,spp", "Samples per pixel", cxxopts::value<int>()->default_value("30"))(
      "w,width", "Set width of the render",
      cxxopts::value<int>()->default_value("960"));

  // parse commandline options
  auto result = opts.parse(argc, argv);
  if (result.count("help")) {
    std::cout << opts.help() << std::endl;
    return 0;
  }
  if (result.count("spp")) {
    samplesPerPixel = result["spp"].as<int>();
  }
  if (result.count("width")) {
    imageWidth = result["width"].as<int>();
    imageHeight = static_cast<int>(imageWidth / aspectRatio);
  }

  // create the output image file
  outputFile.open(result["output"].as<std::string>(), std::ios::out);

  // world
  auto world = random_scene();
  Camera cam(/*lookfrom*/ Point3(13., 2., 3.),
             /*lookat*/ Point3(0., 0., 0.), /*vup*/ Vec3(0., 1., 0.), 20,
             aspectRatio, 0.1, 10.0);

  // render
  std::cerr << "Resolution: " << imageWidth << " x " << imageHeight << '\n';
  std::cerr << "Samples per pixel: " << samplesPerPixel << '\n';
  std::cerr << "Aspect ratio: " << aspectRatio << '\n';
  outputFile << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";
  for (int j = imageHeight - 1; j >= 0; --j) {
    std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
    for (int i = 0; i < imageWidth; ++i) {
      Color pixelColor(0, 0, 0);
      for (int s = 0; s < samplesPerPixel; ++s) {
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
