target_sources(FrontierUnitTests
   PRIVATE
   SettingsClientTest.cpp
   SettingsServiceTest.cpp
)

target_include_directories(FrontierUnitTests
   PRIVATE
   ${CMAKE_SOURCE_DIR}/DataAccess/SettingsService
)

target_link_libraries(FrontierUnitTests
   PUBLIC
   DataAccess
   TestSupport::DataAccessSupport
   TestSupport::TestUtilities
)
