#pragma once

#include "Camera.h"
#include "Color.h"
#include "HittableList.h"
#include "Ray.h"
#include <fstream>
#include <iostream>
#include <vector>

struct Image {
public:
  void printInfo();
  void render(Camera &cam, HittableList &world, int maxDepth);

private:
  Color ray_color(const Ray<double> &r, const Hittable &world, int depth);

public:
  double aspectRatio;
  int width;
  int height;
  int samplesPerPixel;
  std::vector<Color> data; // [r0,g0,b0,r1,g1,b1, ..., r(n-1),g(n-1),b(n-1)]
};

std::ostream &operator<<(std::ostream &out, const Image &img);
