set(SettingsServiceSources
   Settings.h
   SettingsClient.cpp
   SettingsClient.h
   SettingsClientInterface.h
   SettingsService.cpp
   SettingsService.h
)

add_library(SettingsService ${SettingsServiceSources})
target_include_directories(SettingsService PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(SettingsService PRIVATE
   Shared
   Qt${QT_VERSION_MAJOR}::Core
   Qt${QT_VERSION_MAJOR}::Sql
)
