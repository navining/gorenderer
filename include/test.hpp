#include "Raster.h"
using namespace CELL;

void draw(Raster &raster, Image *image) {
	raster.drawImage(100, 100, image);
}
