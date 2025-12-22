#pragma once

#include <Utilities/EnumHelpers.h>

#include <climits>

namespace Settings
{
   Q_NAMESPACE

   enum class Setting: int
   {
      None = 0,
      WallpaperSchedule,
      WallpaperImagePaths,
      WallpaperColor,
      WallpaperDuration,
      WallpaperActiveMode,
      TestSetting = INT_MAX
   };
   Q_ENUM_NS(Setting)

   inline QString ToString(Setting value)
   {
      return EnumToString<Setting>(value);
   }

   inline Setting ToSetting(const QString& str)
   {
      return StringToEnum<Setting>(str);
   }
}
