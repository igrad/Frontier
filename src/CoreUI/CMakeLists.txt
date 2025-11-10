set(CoreUISources
    mainwindow.cpp
    mainwindow.h
)

add_library(CoreUI ${CoreUISources})
target_include_directories(CoreUI PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(CoreUI PRIVATE
   Qt${QT_VERSION_MAJOR}::Core
   Qt${QT_VERSION_MAJOR}::Gui
   Qt${QT_VERSION_MAJOR}::Widgets
)
