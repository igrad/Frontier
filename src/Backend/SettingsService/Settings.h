#pragma once

#include <QMap>
#include <climits>

namespace Settings
{
   enum class Setting: int
   {
      None = 0,
      WallpaperSchedule,
      TestSetting = INT_MAX
   };
}

namespace
{
   using namespace Settings;
   // NOLINTNEXTLINE
   const QMap<Setting, QString> STRING_MAP = {
      { Setting::None, "None" },
      { Setting::WallpaperSchedule, "WallpaperSchedule" },
      { Setting::TestSetting, "TestSetting" }
   };

   QMap<QString, Setting> BuildReverseMap()
   {
      QMap<QString, Setting> rmap;

      for(const QPair<const Setting, const QString>& pair : STRING_MAP.asKeyValueRange())
      {
         rmap[pair.second] = pair.first;
      }

      return rmap;
   }
   const QMap<QString, Setting> STRING_REVERSE_MAP = BuildReverseMap();
}

namespace Settings
{
   inline QString ToString(Setting setting)
   {
      return STRING_MAP.value(setting, "");
   }

   inline Setting FromString(const QString& str)
   {
      return STRING_REVERSE_MAP.value(str, Setting::None);
   }
}
