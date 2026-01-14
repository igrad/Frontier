#pragma once

#include <QRect>

#include <cstdint>

struct DisplayInfo
{
   uint8_t ID;
   bool IsPrimary;
   QRect Rect;
   uint64_t XDPI;
   uint64_t YDPI;

   bool operator==(const DisplayInfo& rhs) const
   {
      return ID == rhs.ID &&
             IsPrimary == rhs.IsPrimary &&
             Rect == rhs.Rect &&
             XDPI == rhs.XDPI &&
             YDPI == rhs.YDPI;
   }
};
