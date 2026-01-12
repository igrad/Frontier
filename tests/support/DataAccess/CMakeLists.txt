set(DataAccessSupportSources
   AssetLoader/AssetLoaderMock.cpp
   AssetLoader/AssetLoaderMock.h

   SettingsService/SettingsClientMock.h
   SettingsService/SettingsServiceMock.cpp
   SettingsService/SettingsServiceMock.h
   SettingsService/SettingsServicePointerHelper.h
)

add_library(DataAccessSupport
   ${DataAccessSupportSources}
)
add_library(TestSupport::DataAccessSupport ALIAS DataAccessSupport)

target_include_directories(DataAccessSupport
   PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_SOURCE_DIR}/DataAccess/AssetLoader
   ${CMAKE_SOURCE_DIR}/DataAccess/SettingsService
)

target_link_libraries(DataAccessSupport
   PUBLIC
   DataAccess
   Qt6::Core
   Qt6::Sql
   GTest::gmock
)
