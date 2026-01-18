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
   QSet<uint8_t> AffectedDisplays;
};

Q_DECLARE_METATYPE(DisplayEvent);

struct DisplayInfo
{
   QString ID;
   QString Name;
   HMONITOR Handle;
   uint8_t Number;
   bool IsPrimary;
   QRect Rect;
   uint64_t XDPI;
   uint64_t YDPI;

   bool operator==(const DisplayInfo& rhs) const
   {
      return ID == rhs.ID &&
             Name == rhs.Name &&
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
   return qHashMulti(seed, key.Name, key.Number);
}
