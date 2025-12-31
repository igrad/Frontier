#pragma once

#include <Utilities/EnumHelpers.h>

#include <QColor>
#include <QString>

namespace Wallpaper
{
   Q_NAMESPACE

   enum class Fit: int
   {
      None = 0,
      Fill,
      Fit,
      Stretch,
      Tile,
      Center,
      Span
   };
   Q_ENUM_NS(Fit)

   inline QString ToString(Fit fit)
   {
      return EnumToString<Fit>(fit);
   }

   enum class Schedule: int
   {
      None = 0,
      Static,
      Sequence,
      Shuffle
   };
   Q_ENUM_NS(Schedule)

   inline QString ToString(Schedule schedule)
   {
      return EnumToString<Schedule>(schedule);
   }

   enum class Style: int
   {
      None = 0,
      StaticColor,
      DynamicColor,
      Image,
      Video
   };
   Q_ENUM_NS(Style)

   inline QString ToString(Style style)
   {
      return EnumToString<Style>(style);
   }

   struct ViewData
   {
      quint8 AssignedMonitor;
      Fit Fit;
      Style Style;
      QColor Color;
      QString ImagePath;
   };
}

Q_DECLARE_METATYPE(Wallpaper::ViewData);
