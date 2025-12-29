add_subdirectory(SettingsService)

target_include_directories(FrontierUnitTests PRIVATE
   ${CMAKE_CURRENT_SOURCE_DIR}
)
