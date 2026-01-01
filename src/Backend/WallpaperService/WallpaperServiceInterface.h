#pragma once

#include "WallpaperTypes.h"

#include <QObject>

namespace Wallpaper
{
   class WallpaperServiceInterface: public QObject
   {
      Q_OBJECT

   public:
      ~WallpaperServiceInterface() = default;

      virtual void RegisterMetaTypes() const = 0;

   signals:
      void WallpaperDataChanged(const Wallpaper::ViewData& data);
   };
}
