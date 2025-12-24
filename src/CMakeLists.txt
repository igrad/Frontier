add_subdirectory(Backend)
add_subdirectory(Shared)
add_subdirectory(UI)

set(PROJECT_SOURCES
   main.cpp
)

qt_add_executable(Frontier
   MANUAL_FINALIZATION
   ${PROJECT_SOURCES}
)

find_package(Qt6 REQUIRED COMPONENTS Multimedia MultimediaWidgets)

target_link_libraries(Frontier
   PRIVATE
   CoreUI
   BackendThreadManager
   SettingsService
   Qt6::Core
   Qt6::Gui
   Qt6::Multimedia
   Qt6::MultimediaWidgets
   Qt6::Widgets
)

if(${Qt6_VERSION} VERSION_LESS 6.1.0)
   set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.FrontierLauncher)
endif()

set_target_properties(Frontier PROPERTIES
   ${BUNDLE_ID_OPTION}
   WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Frontier
   BUNDLE DESTINATION .
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(Qt6_VERSION_MAJOR EQUAL 6)
   qt_finalize_executable(Frontier)
endif()
