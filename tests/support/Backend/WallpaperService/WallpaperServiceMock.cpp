#include "WallpaperServiceMock.h"

using namespace Wallpaper;

void WallpaperServiceMock::EmitWallpaperDataChanged(uint8_t display, const ViewData& data)
{
   emit WallpaperDataChanged(display, data);
}
