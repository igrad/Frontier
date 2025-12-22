#pragma once

#include <Utilities/EnumHelpers.h>

#include <QString>

namespace Wallpaper
{
   Q_NAMESPACE

   enum class WallpaperMode: int
   {
      None = 0,
      StaticColor,
      DynamicColor,
      StaticImage,
      Video
   };
   Q_ENUM_NS(WallpaperMode);

   inline QString ToString(WallpaperMode value)
   {
      return EnumToString(value);
   }

   inline WallpaperMode FromString(const QString& str)
   {
      return StringToEnum<WallpaperMode>(str);
   }

   enum class WallpaperSchedule: int
   {
      None = 0,
      Static,
      Sequence,
      Shuffle
   };
   Q_ENUM_NS(WallpaperSchedule)

   inline QString ToString(WallpaperSchedule value)
   {
      return EnumToString<WallpaperSchedule>(value);
   }
}
