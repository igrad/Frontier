#pragma once

#include "WallpaperServiceInterface.h"
#include "WallpaperSettingsProxy.h"

#include <DisplayInfo.h>
#include <Utilities/Rando.h>

#include <QTimer>
#include <Timer.h>

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
      void HandleDisplaysDetected(const QList<DisplayInfo>& info) override;

   private slots:
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

      WallpaperSettingsProxy SettingsProxy;

      QList<ViewData> CurrentData;
      int CurrentColorsIndex;
      int CurrentImageIndex;
      Timer RotationTimer;
      Rando ShuffleRando;
   };
}
