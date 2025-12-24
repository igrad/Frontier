set(CoreUISources
   ShellWindow.cpp
   ShellWindow.h
   UIManager.cpp
   UIManager.h

   Wallpaper/DynamicColorWallpaper.cpp
   Wallpaper/DynamicColorWallpaper.h
   Wallpaper/WallpaperView.cpp
   Wallpaper/WallpaperView.h
)

add_library(CoreUI ${CoreUISources})
target_include_directories(CoreUI PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_SOURCE_DIR}/src
   ${CMAKE_SOURCE_DIR}/src/Backend
   ${CMAKE_SOURCE_DIR}/src/Shared
)

find_package(Qt6 REQUIRED COMPONENTS Multimedia MultimediaWidgets)

target_link_libraries(CoreUI PRIVATE
   Qt6::Core
   Qt6::Gui
   Qt6::Multimedia
   Qt6::MultimediaWidgets
   Qt6::Widgets
   Shared
   WallpaperService
   SharedUIComponents
)
