set(SettingsServiceTestSources
   SettingsClientTest.cpp
   SettingsServiceTest.cpp
)

add_library(SettingsServiceTests
   ${SettingsServiceTestSources}
)

target_include_directories(SettingsServiceTests
   PUBLIC
   ${CMAKE_SOURCE_DIR}/DataAccess/SettingsService
)

target_link_libraries(SettingsServiceTests PRIVATE
   Shared
   TestSupport::TestMain
   TestSupport::DataAccessSupport
   TestSupport::TestUtilities
   Qt6::Core
   Qt6::Gui
   Qt6::Sql
   Qt6::Test
)
