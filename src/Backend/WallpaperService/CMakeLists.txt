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
   ${CMAKE_SOURCE_DIR}/src
   ${CMAKE_SOURCE_DIR}/src/DataAccess
)

target_link_libraries(WallpaperService PUBLIC
   DataAccess
   Qt6::Gui
   PRIVATE
   Shared
   Qt6::Core
)
