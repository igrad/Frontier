set(TaskBarServiceSources
   TaskBarService.cpp
   TaskBarService.h
   TaskBarServiceInterface.h
   TaskBarServiceWorker.cpp
   TaskBarServiceWorker.h
   TaskBarSettingsProxy.cpp
   TaskBarSettingsProxy.h
   TaskBarSettingsProxyInterface.h
   TaskBarTypes.h
)

add_library(TaskBarService ${TaskBarServiceSources})
target_include_directories(TaskBarService PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_SOURCE_DIR}/src
   ${CMAKE_SOURCE_DIR}/src/DataAccess
)

target_link_libraries(TaskBarService PRIVATE
   DataAccess
   DisplaysManager
   Shared
   Qt6::Gui
   Qt6::Core
)
