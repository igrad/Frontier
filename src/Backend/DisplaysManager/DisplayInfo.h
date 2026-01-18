#pragma once

#include <QMetaType>
#include <QRect>
#include <QSet>

#include <cstdint>
#include <windef.h>

struct DisplayEvent
{
   enum class EventType: int
   {
      None = 0,
      Added,
      Removed
   };

   EventType Event;
   QSet<QString> AffectedDisplays;
};

Q_DECLARE_METATYPE(DisplayEvent);

struct DisplayInfo
{
   QString ID = "";				// The EDID of the display hardware
   QString SessionName = "";	// The name assigned to this monitor for this session
   QString DisplayName = "";	// The name of the monitor itself
   HMONITOR Handle;				// The handle used to fetch display info in windows API
   uint8_t Number = 0;			// Number currently assigned to this display
   bool IsPrimary = false;
   QRect Rect = {0, 0, 0, 0};
   uint64_t XDPI = 1;			// 1 just to prevent divide by 0 errors
   uint64_t YDPI = 1;

   bool operator==(const DisplayInfo& rhs) const
   {
      return ID == rhs.ID &&
             SessionName == rhs.SessionName &&
             DisplayName == rhs.DisplayName &&
             Handle == rhs.Handle &&
             Number == rhs.Number &&
             IsPrimary == rhs.IsPrimary &&
             Rect == rhs.Rect &&
             XDPI == rhs.XDPI &&
             YDPI == rhs.YDPI;
   }

   bool operator!=(const DisplayInfo& rhs) const
   {
      return !(*this == rhs);
   }
};

Q_DECLARE_METATYPE(DisplayInfo);

inline size_t qHash(const DisplayInfo& key, size_t seed = 0)
{
   return qHashMulti(seed, key.SessionName, key.Number);
}
