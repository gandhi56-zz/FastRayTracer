#include "Camera.h"
#include "Color.h"
#include "HittableList.h"
#include "Image.h"
#include "Material.h"
#include "RTWeekend.h"
#include "Ray.h"
#include "Sphere.h"
#include "Vec3.h"
#include "cxxopts.hpp"
#include <CL/cl.h>
#include <cstdlib>
#include <fstream>
#include <iostream>

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
  ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id,
                       &ret_num_devices);

  // Create an image
  Image img{3. / 2., 960, 640, 30};

  // Add commandline options
  cxxopts::Options opts(argv[0], "FastRayTracer by Anshil\n");
  opts.add_options()("h,help", "Print usage")(
      "o,output", "Write output to ",
      cxxopts::value<std::string>()->default_value("render.ppm"))(
      "s,spp", "Samples per pixel", cxxopts::value<int>()->default_value("30"))(
      "w,width", "Set width of the render",
      cxxopts::value<int>()->default_value("960"));

  // Parse commandline options
  auto result = opts.parse(argc, argv);
  if (result.count("help")) {
    std::cout << opts.help() << std::endl;
    return 0;
  }
  if (result.count("spp")) {
    img.samplesPerPixel = result["spp"].as<int>();
  }
  if (result.count("width")) {
    img.width = result["width"].as<int>();
    img.height = static_cast<int>(img.width / img.aspectRatio);
  }

  // Create the output img file
  std::ofstream outputFile(result["output"].as<std::string>(), std::ios::out);

  // World
  auto world = random_scene();

  // Set up camera
  Camera cam(/*lookfrom*/ Point3(13., 2., 3.),
             /*lookat*/ Point3(0., 0., 0.), /*vup*/ Vec3(0., 1., 0.), 20,
             img.aspectRatio, 0.1, 10.0);

  // Render scene
  img.printInfo();
  img.render(cam, world, 50);
  outputFile << img;
  std::cerr << "\nImage file " << result["output"].as<std::string>()
            << " was created.\n";
  return 0;
}
