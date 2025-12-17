set(WallpaperServiceSources
   WallpaperService.cpp
   WallpaperService.h

   StaticColorWallpaper.cpp
   StaticColorWallpaper.h
   DynamicColorWallpaper.cpp
   DynamicColorWallpaper.h
   StaticImageWallpaper.cpp
   StaticImageWallpaper.h
   VideoWallpaper.cpp
   VideoWallpaper.h
   WallpaperTypes.h
)

add_library(WallpaperService ${WallpaperServiceSources})
target_include_directories(WallpaperService PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(WallpaperService PUBLIC
   SettingsService
   PRIVATE
   Shared
   Qt6::Core
)
