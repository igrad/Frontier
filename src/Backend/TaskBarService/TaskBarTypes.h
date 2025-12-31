#pragma once

#include <Utilities/EnumHelpers.h>

#include <QString>

namespace TaskBar
{
   Q_NAMESPACE

   // The edge of the monitor to which the taskbar will align
   enum class Alignment: int
   {
      None = 0,
      Bottom,
      Left,
      Top,
      Right
   };
   Q_ENUM_NS(Alignment);

   inline QString ToString(Alignment value)
   {
      return EnumToString(value);
   }

   // The directional flow of elements of the task bar
   enum class Orientation: int
   {
      None = 0,
      LeftToRight,
      Centered,
      RightToLeft
   };
   Q_ENUM_NS(Orientation);

   inline QString ToString(Orientation value)
   {
      return EnumToString(value);
   }

   // Necessary data to display a task bar on-screen
   struct ViewData
   {
      quint8 AssignedMonitor;
      Alignment Alignment;
      Orientation Orientation;
      double Opacity;
      bool AutoHide;
      int AutoHideDelayMs;
   };
}

Q_DECLARE_METATYPE(TaskBar::ViewData);
