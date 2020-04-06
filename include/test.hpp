#include "Raster.h"
using namespace CELL;

void draw(Raster &raster, Image *image) {
	raster.drawImageScaleHighQuality(0, 0, 500, 500, image);
	raster.drawImageScale(400, 0, 500, 500, image);
}
