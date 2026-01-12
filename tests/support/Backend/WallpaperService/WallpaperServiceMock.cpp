#include "WallpaperServiceMock.h"

using namespace Wallpaper;

void WallpaperServiceMock::EmitWallpaperDataChanged(const ViewData& data)
{
   emit WallpaperDataChanged(data);
}
