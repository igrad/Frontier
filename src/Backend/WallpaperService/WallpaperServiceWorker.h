#pragma once

#include "WallpaperTypes.h"

#include <DisplayInfo.h>
#include <Timer.h>
#include <Utilities/Rando.h>

namespace Wallpaper
{
   class WallpaperSettingsProxy;
   class WallpaperServiceWorker: public QObject
   {
      Q_OBJECT

   public:
      WallpaperServiceWorker(const DisplayInfo& info,
              WallpaperSettingsProxy& settingsProxy,
              QObject* parent = nullptr);
      WallpaperServiceWorker(const WallpaperServiceWorker& other);
      ~WallpaperServiceWorker() = default;

      uint8_t GetDisplayNum() const;
      DisplayInfo GetDisplayInfo() const;

      bool operator==(const WallpaperServiceWorker& rhs) const;

   signals:
      void WallpaperViewDataChanged(uint8_t display,
                                    const Wallpaper::ViewData& data);

   public slots:
      void HandleRotationTimeout();
      void HandleSettingsChanged();

   private:
      void CalculateCurrentWallpaperData(bool triggeredByRotationTimer = false);
      void CalculateNextColor(bool shuffled);
      void CalculateNextImage(bool shuffled);
      void CalculateSequenceOrShuffleViewData(bool triggeredByTimer);
      void CalculateStaticViewData();
      void ProcessColorStyle(bool triggeredByTimer);
      void ProcessImageStyle(bool triggeredByTimer);

      uint8_t DisplayNum;
      DisplayInfo Info;
      WallpaperSettingsProxy& SettingsProxy;

      ViewData Data;
      int CurrentColorsIndex;
      int CurrentImageIndex;
      Timer RotationTimer;
      Rando ShuffleRando;
   };

   inline size_t qHash(const Wallpaper::WallpaperServiceWorker& key, size_t seed = 0)
   {
      return qHashMulti(seed, key.GetDisplayNum(), key.GetDisplayInfo().Name);
   }
}

