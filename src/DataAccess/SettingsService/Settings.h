#pragma once

#include <Utilities/EnumHelpers.h>

#include <climits>

namespace Settings
{
   Q_NAMESPACE

   enum class Setting: int
   {
      None = 0,
      TaskBarAlignment,
      TaskBarOrientation,
      WallpaperColors,
      WallpaperDuration,
      WallpaperFits,
      WallpaperImagePaths,
      WallpaperSchedule,
      WallpaperStyle,
      _All,							// Keep as penultimate
      _TestSetting = INT_MAX	// Must be last
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
