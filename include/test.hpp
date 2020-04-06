#include "Raster.h"
using namespace CELL;

void draw(Raster &raster, Image *image) {
	raster.drawImageAlphaTest(0, 0, image, 1000);
	raster.drawImageAlphaBlend(200, 0, image, 0.5);
	raster.drawImageAlpha(400, 0, image, 0.5);
	raster.drawImage(600, 0, image);
}
