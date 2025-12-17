#pragma once

#include <QObject>

namespace wp
{
   // A static wallpaper with a single color
   class StaticColorWallpaper: public QObject
   {
      Q_OBJECT

   public:
      StaticColorWallpaper();
   };
}
