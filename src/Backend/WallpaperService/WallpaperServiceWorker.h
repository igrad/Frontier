#pragma once

#include "WallpaperTypes.h"

#include <DisplayInfo.h>
#include <Timer.h>
#include <Utilities/Rando.h>

namespace Wallpaper
{
   class WallpaperSettingsProxyInterface;
   class WallpaperServiceWorker: public QObject
   {
      Q_OBJECT

   public:
      WallpaperServiceWorker(const DisplayInfo& info,
              WallpaperSettingsProxyInterface* settingsProxy,
              QObject* parent = nullptr);
      WallpaperServiceWorker(const WallpaperServiceWorker& other);
      ~WallpaperServiceWorker() = default;

      QString GetDisplayID() const;
      DisplayInfo GetDisplayInfo() const;

      bool operator==(const WallpaperServiceWorker& rhs) const;
      WallpaperServiceWorker& operator=(const WallpaperServiceWorker& rhs);

   signals:
      void WallpaperViewDataChanged(const DisplayID& id,
                                    const Wallpaper::ViewData& data);

   public slots:
      void HandleRotationTimeout();
      void HandleSettingsChanged();
      void HandleDisplayConfigChanged(const DisplayInfo& info);
      void HandleDisplayRemoved(const DisplayInfo& info);

   private:
      void CalculateCurrentWallpaperData(bool triggeredByRotationTimer = false);
      void CalculateNextColor(bool shuffled);
      void CalculateNextImage(bool shuffled);
      void CalculateSequenceOrShuffleViewData(bool triggeredByTimer);
      void CalculateStaticViewData();
      void ProcessColorStyle(bool triggeredByTimer);
      void ProcessImageStyle(bool triggeredByTimer);

      DisplayID Display;
      DisplayInfo Info;
      WallpaperSettingsProxyInterface* SettingsProxy;

      ViewData Data;
      int CurrentColorsIndex;
      int CurrentImageIndex;
      Timer RotationTimer;
      Rando ShuffleRando;
   };
}

