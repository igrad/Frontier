#pragma once

#include "WallpaperTypes.h"

#include <QObject>

struct DisplayInfo;

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
      virtual void HandleDisplaysDetected(const QList<DisplayInfo>& info) = 0;
   };
}
