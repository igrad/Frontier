target_sources(FrontierUnitTests
   PRIVATE
   TaskBarTests/TaskBarViewTests.cpp
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
   TestSupport::BackendSupport
   TestSupport::DataAccessSupport
   TestSupport::TestUtilities
   TestSupport::UISupport
)
