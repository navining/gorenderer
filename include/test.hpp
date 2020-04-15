#include "Raster.h"
using namespace CELL;

void draw(Raster &raster, Image *image) {
  float2 p0(50, 50);
  float2 p1(200, 50);
  float2 p2(33, 88);
  float2 p3(150, 150);

  raster.drawBezier(p0, p3, p1, p2);
}
