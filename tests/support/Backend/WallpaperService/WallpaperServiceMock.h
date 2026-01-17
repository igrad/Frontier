#pragma once

#include <WallpaperServiceInterface.h>

#include <gmock/gmock.h>

namespace Wallpaper
{
   class WallpaperServiceMock: public WallpaperServiceInterface
   {
      Q_OBJECT

   public:
      WallpaperServiceMock() = default;
      ~WallpaperServiceMock() = default;

      MOCK_METHOD(void, RegisterMetaTypes, (), (const, override));

      void EmitWallpaperDataChanged(uint8_t display, const Wallpaper::ViewData& data);
   };
}
