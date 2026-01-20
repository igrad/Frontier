#include "WallpaperServiceMock.h"

using namespace Wallpaper;

void WallpaperServiceMock::EmitWallpaperDataChanged(const DisplayID& displayID, const ViewData& data)
{
   emit WallpaperDataChanged(displayID, data);
}
