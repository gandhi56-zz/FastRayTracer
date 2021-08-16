#include "Vec3.h"
#include "Color.h"
#include <iostream>

int main(){
  const int image_width = 1280;
  const int image_height = 720;

  // render
  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
  for (int j = image_height - 1; j >= 0; --j){
    std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
    for (int i = 0; i < image_width; ++i){
      Color pixel_color(double(i)/(image_width-1), double(j)/(image_height-1), 0.25);
      write_color(std::cout, pixel_color);
    }
  }
  return 0;
}
