#include "WallpaperSettingsProxyMock.h"

using namespace Wallpaper;

void WallpaperSettingsProxyMock::EmitSettingsChanged(const QString& displayID)
{
   emit SettingsChanged(displayID);
}
