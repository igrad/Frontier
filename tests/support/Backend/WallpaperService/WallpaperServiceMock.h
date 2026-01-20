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
      MOCK_METHOD(void,
                  HandleDisplayConfigChanged,
                  (const DisplayConfigEvent&),
                  (override));

      void EmitWallpaperDataChanged(const DisplayID& displayID,
                                    const Wallpaper::ViewData& data);
   };
}
