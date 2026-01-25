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
   enum class Orientation: int
   {
      None = 0,
      Centered,
      LeftToRight,
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
      Alignment Alignment;
      QString DisplayID;
      bool AutoHide;
      int AutoHideDelayMs;
      Orientation Orientation;
      double Opacity;
      QRect Rect;
      bool Shown;
      QString StartButtonImagePath;
      QRect StartButtonRect;
      bool StartButtonShown;

      bool operator==(const ViewData& rhs) const
      {
         return Alignment == rhs.Alignment &&
                DisplayID == rhs.DisplayID &&
                AutoHide == rhs.AutoHide &&
                AutoHideDelayMs == rhs.AutoHideDelayMs &&
                Orientation == rhs.Orientation &&
                Opacity == rhs.Opacity &&
                Shown == rhs.Shown &&
                StartButtonImagePath == rhs.StartButtonImagePath &&
                StartButtonRect == rhs.StartButtonRect &&
                StartButtonShown == rhs.StartButtonShown;
      }
   };

}

Q_DECLARE_METATYPE(TaskBar::ViewData);
