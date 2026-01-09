set(WallpaperServiceTestSources
   WallpaperServiceTest.cpp
   WallpaperSettingsProxyTest.cpp
)

add_library(WallpaperServiceTests
   ${WallpaperServiceTestSources}
)

target_include_directories(WallpaperServiceTests
   PUBLIC
   ${CMAKE_SOURCE_DIR}/Backend/WallpaperService
)

target_link_libraries(WallpaperServiceTests PRIVATE
   Shared
   WallpaperService
   TestSupport::BackendSupport
   TestSupport::DataAccessSupport
   TestSupport::TestUtilities
   Qt6::Core
   Qt6::Gui
   Qt6::Sql
   Qt6::Test
)
