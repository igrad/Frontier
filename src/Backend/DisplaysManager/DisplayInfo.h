#pragma once

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

struct DisplayInfo
{
   HMONITOR Handle;
   uint8_t Number;
   bool IsPrimary;
   QRect Rect;
   uint64_t XDPI;
   uint64_t YDPI;

   bool operator==(const DisplayInfo& rhs) const
   {
      return Handle == rhs.Handle &&
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
