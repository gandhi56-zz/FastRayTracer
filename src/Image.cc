#include "Image.h"
#include "Material.h"

Color Image::ray_color(const Ray<double> &r, const Hittable &world, int depth) {
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

void Image::render(Camera &cam, HittableList &world, int maxDepth) {
  data.resize(height * width);
  for (int j = height - 1; j >= 0; --j) {
    for (int i = 0; i < width; ++i) {
      std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
      for (int s = 0; s < samplesPerPixel; ++s) {
        auto u = (i + random_t<double>()) / (width - 1);
        auto v = (j + random_t<double>()) / (height - 1);
        Ray<double> r = cam.getRay(u, v);
        data[i + j * width] += ray_color(r, world, maxDepth);
      }
    }
  }
}

void Image::printInfo() {
  std::cerr << "Resolution: " << width << " x " << height << '\n';
  std::cerr << "Samples per pixel: " << samplesPerPixel << '\n';
  std::cerr << "Aspect ratio: " << aspectRatio << '\n';
}

std::ostream &operator<<(std::ostream &out, const Image &img) {
  out << "P3\n" << img.width << ' ' << img.height << "\n255\n";
  for (auto it = img.data.rbegin(); it != img.data.rend(); ++it)
    write_color(out, *it, img.samplesPerPixel);
  return out;
}
