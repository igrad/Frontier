#pragma once

#include <QObject>

namespace Wallpaper
{
   // A static wallpaper with a single color
   class StaticColorWallpaper: public QObject
   {
      Q_OBJECT

   public:
      StaticColorWallpaper() = default;
      ~StaticColorWallpaper() = default;
   };
}
