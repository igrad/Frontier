#pragma once

#include <Utilities/EnumHelpers.h>

#include <QRect>
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
   enum class Direction: int
   {
      None = 0,
      Centered,
      LeftToRight,
      RightToLeft
   };
   Q_ENUM_NS(Direction);

   inline QString ToString(Direction value)
   {
      return EnumToString(value);
   }

   // Necessary data to display a task bar on-screen
   struct ViewData
   {
      QString DisplayID;
      Alignment Alignment;
      bool AutoHide;
      int AutoHideDelayMs;
      Direction IconDirection;
      double Opacity;
      QRect Rect;
      bool Shown;
      QString StartButtonImagePath;
      QRect StartButtonRect;
      bool StartButtonShown;
      Direction Direction;
      // auto-hide misc icons bool
      // app icon docks grid dimensions

      bool operator==(const ViewData& rhs) const
      {
         return DisplayID == rhs.DisplayID &&
                Alignment == rhs.Alignment &&
                AutoHide == rhs.AutoHide &&
                AutoHideDelayMs == rhs.AutoHideDelayMs &&
                Direction == rhs.Direction &&
                Opacity == rhs.Opacity &&
                Shown == rhs.Shown &&
                IconDirection == rhs.IconDirection &&
                StartButtonImagePath == rhs.StartButtonImagePath &&
                StartButtonRect == rhs.StartButtonRect &&
                StartButtonShown == rhs.StartButtonShown;
      }
   };
}

Q_DECLARE_METATYPE(TaskBar::ViewData);
