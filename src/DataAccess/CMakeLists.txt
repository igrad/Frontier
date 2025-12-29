set(DataAccessSources
   DataAccessThreadManager.cpp
   DataAccessThreadManager.h
)

set(SettingsServiceSources
   SettingsService/Settings.h
   SettingsService/SettingsClient.cpp
   SettingsService/SettingsClient.h
   SettingsService/SettingsClientInterface.h
   SettingsService/SettingsDbHelpers.h
   SettingsService/SettingsService.cpp
   SettingsService/SettingsService.h
   SettingsService/SettingsServiceInterface.h
)

add_library(DataAccess STATIC
   ${DataAccessSources}
   ${SettingsServiceSources}
)

target_include_directories(DataAccess PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/SettingsService
)

target_link_libraries(DataAccess PRIVATE
   Shared
   Qt6::Core
   Qt6::Sql
)
