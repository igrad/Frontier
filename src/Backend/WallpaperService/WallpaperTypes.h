#pragma once

namespace Wallpaper
{
   enum class WallpaperMode: int
   {
      None = 0,
      StaticColor,
      DynamicColor,
      StaticImage,
      Video
   };

   enum class WallpaperSchedule: int
   {
      None = 0,
      Static,
      Sequence,
      Shuffle
   };
}
