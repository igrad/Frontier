set(CoreUISources
   ShellWindow.cpp
   ShellWindow.h

   Wallpaper/WallpaperView.cpp
   Wallpaper/WallpaperView.h
)

add_library(CoreUI ${CoreUISources})
target_include_directories(CoreUI PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(CoreUI PRIVATE
   Qt6::Core
   Qt6::Gui
   Qt6::Widgets
   Shared
   SharedUIComponents
)
