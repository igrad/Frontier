set(BackendSupportSources
   TaskBarService/TaskBarServiceMock.h
   WallpaperService/WallpaperServiceMock.h
)

add_library(BackendSupport
   ${BackendSupportSources}
)
add_library(TestSupport::BackendSupport ALIAS BackendSupport)

target_include_directories(BackendSupport
   PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(BackendSupport
   PUBLIC
   Qt6::Core
   Qt6::Widgets
)
