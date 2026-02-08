target_sources(FrontierUnitTests
   PRIVATE
   ShellUITests.cpp
   ShellWindowTests.cpp
   UIManagerTests.cpp

   TaskBarTests/TaskBarViewTests.cpp
   TaskBarTests/TaskBarAssetProxyTests.cpp

   WallpaperTests/WallpaperViewTests.cpp
)

target_include_directories(FrontierUnitTests
   PRIVATE
   ${CMAKE_SOURCE_DIR}/src/UI/CoreUI
)

target_link_libraries(FrontierUnitTests
   PUBLIC
   CoreUI
   TaskBarService
   WallpaperService
   WindowsAPI
   TestSupport::BackendSupport
   TestSupport::DataAccessSupport
   TestSupport::TestUtilities
   TestSupport::UISupport
)
