set(BackendThreadManagerSources
   BackendThreadManager.cpp
   BackendThreadManager.h
)

add_library(BackendThreadManager ${BackendThreadManagerSources})
target_include_directories(BackendThreadManager PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(BackendThreadManager PUBLIC
   PRIVATE
   SettingsService
   Shared
   WallpaperService
   Qt6::Core
)
