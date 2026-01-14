#pragma once

#include <Utilities/EnumHelpers.h>

#include <climits>

namespace Windows
{
   Q_NAMESPACE

   enum class Setting: int
   {
      None = 0,
      NumberOfDetectedMonitors,
      _All,							// Keep as penultimate
      _TestSetting = INT_MAX	// Must be last
   };
   Q_ENUM_NS(Windows::Setting)

   inline QString ToString(Windows::Setting value)
   {
      return EnumToString<Windows::Setting>(value);
   }
}

Q_DECLARE_METATYPE(Windows::Setting)
