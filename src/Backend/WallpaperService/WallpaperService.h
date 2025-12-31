#pragma once

#include "WallpaperTypes.h"
#include "WallpaperSettingsProxy.h"

#include <Utilities/Rando.h>

#include <QTimer>

namespace Wallpaper
{
   class WallpaperService: public QObject
   {
      Q_OBJECT

   public:
      explicit WallpaperService(QObject* parent = nullptr);
      ~WallpaperService() = default;

      void RegisterMetaTypes() const;

   signals:
      void WallpaperDataChanged(const Wallpaper::ViewData& data);

   private slots:
      void HandleRotationTimeout();
      void HandleSettingsChanged();

   private:
      void CalculateCurrentWallpaperData(bool triggeredByRotationTimer = false);
      void CalculateNextColor(bool shuffled);
      void CalculateNextImage(bool shuffled);
      void CalculateSequenceOrShuffleViewData(ViewData& data, bool triggeredByTimer);
      void CalculateStaticViewData(ViewData& data);

      WallpaperSettingsProxy SettingsProxy;

      int CurrentColorsIndex;
      int CurrentImageIndex;
      QTimer RotationTimer;
      Rando ShuffleRando;
   };
}
