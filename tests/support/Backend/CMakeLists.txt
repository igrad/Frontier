set(BackendSupportSources
   TaskBarService/TaskBarServiceMock.cpp
   TaskBarService/TaskBarServiceMock.h

   WallpaperService/WallpaperServiceMock.cpp
   WallpaperService/WallpaperServiceMock.h
   WallpaperService/WallpaperSettingsProxyMock.cpp
   WallpaperService/WallpaperSettingsProxyMock.h
)

add_library(BackendSupport
   ${BackendSupportSources}
)
add_library(TestSupport::BackendSupport ALIAS BackendSupport)

target_include_directories(BackendSupport
   PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/TaskBarService
   ${CMAKE_CURRENT_SOURCE_DIR}/WallpaperService
)

target_link_libraries(BackendSupport
   PUBLIC
   TaskBarService
   WallpaperService
   Qt6::Core
   Qt6::Widgets
   GTest::gmock
)
