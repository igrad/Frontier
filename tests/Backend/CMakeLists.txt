add_library(BackendTests INTERFACE)

target_link_libraries(BackendTests
   INTERFACE
   WallpaperServiceTests
)

add_subdirectory(WallpaperServiceTests)
