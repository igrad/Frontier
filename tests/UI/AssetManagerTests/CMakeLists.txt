target_sources(FrontierUnitTests
   PRIVATE
   AssetManagerTests.cpp
)

target_include_directories(FrontierUnitTests
   PRIVATE
   ${CMAKE_SOURCE_DIR}/src/UI/AssetManager
)

target_link_libraries(FrontierUnitTests
   PUBLIC
   AssetManager
   TestSupport::BackendSupport
   TestSupport::DataAccessSupport
   TestSupport::TestUtilities
)
