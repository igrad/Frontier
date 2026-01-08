target_sources(FrontierUnitTests
   PRIVATE
   AssetLoaderTests.cpp
)

target_include_directories(FrontierUnitTests
   PRIVATE
   ${CMAKE_SOURCE_DIR}/DataAccess/AssetLoader
)

target_link_libraries(FrontierUnitTests
   PUBLIC
   DataAccess
   TestSupport::DataAccessSupport
   TestSupport::TestUtilities
)
