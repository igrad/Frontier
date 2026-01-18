#pragma once

#include "WallpaperServiceInterface.h"
#include "WallpaperSettingsProxy.h"
#include "WallpaperServiceWorker.h"

#include <DisplayInfo.h>

namespace Wallpaper
{
   class WallpaperService: public WallpaperServiceInterface
   {
      Q_OBJECT

   public:
      explicit WallpaperService(QObject* parent = nullptr);
      ~WallpaperService() = default;

      void RegisterMetaTypes() const override;

   public slots:
      void HandleDisplayConfigChanged(const DisplayEvent& event,
                                      const QSet<DisplayInfo>& displays) override;

   private slots:
      void HandleSettingsChanged();

   private:
      WallpaperSettingsProxy SettingsProxy;
      QList<WallpaperServiceWorker*> Workers;
   };
}
