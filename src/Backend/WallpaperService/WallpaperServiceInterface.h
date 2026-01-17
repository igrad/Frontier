#pragma once

#include "WallpaperTypes.h"

#include <DisplayInfo.h>

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
      void WallpaperDataChanged(uint8_t display, const Wallpaper::ViewData& data);

   public slots:
      virtual void HandleDisplayConfigChanged(const DisplayEvent& event,
                                              const QSet<DisplayInfo>& displays) = 0;
   };
}
