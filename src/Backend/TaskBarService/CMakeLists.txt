set(TaskBarServiceSources
   TaskBarService.cpp
   TaskBarService.h
   TaskBarServiceInterface.h
   TaskBarTypes.h
)

add_library(TaskBarService ${TaskBarServiceSources})
target_include_directories(TaskBarService PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(TaskBarService PUBLIC
   SettingsService
   Qt6::Gui
   PRIVATE
   Shared
   Qt6::Core
)
