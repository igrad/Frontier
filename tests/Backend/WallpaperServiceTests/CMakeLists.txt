target_sources(FrontierUnitTests
   PRIVATE
   WallpaperServiceTest.cpp
   WallpaperSettingsProxyTest.cpp
)

target_include_directories(FrontierUnitTests
   PRIVATE
   ${CMAKE_SOURCE_DIR}/Backend/WallpaperService
)

target_link_libraries(FrontierUnitTests
   PUBLIC
   WallpaperService
   TestSupport::DataAccessSupport
   TestSupport::TestUtilities
)
