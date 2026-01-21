set(EnterpriseSources
   EnterpriseWindow.cpp
   EnterpriseWindow.h
   EnterpriseService.cpp
   EnterpriseService.h

   RowData.h

   Settings/EnterpriseSettingsModel.cpp
   Settings/EnterpriseSettingsModel.h

   Widgets/EnterpriseMonitorWidget.cpp
   Widgets/EnterpriseMonitorWidget.h
   Widgets/EnterpriseMonitorWindow.cpp
   Widgets/EnterpriseMonitorWindow.h
   Widgets/EnterpriseSettingsView.cpp
   Widgets/EnterpriseSettingsView.h

   Win32APIController/EnterpriseWin32APIController.cpp
   Win32APIController/EnterpriseWin32APIController.h
)

add_library(Enterprise ${EnterpriseSources})
target_include_directories(Enterprise PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_SOURCE_DIR}/src
   ${CMAKE_SOURCE_DIR}/src/Backend
   ${CMAKE_SOURCE_DIR}/src/Shared
   ${CMAKE_SOURCE_DIR}/src/DataAccess
)

target_link_libraries(Enterprise PRIVATE
   DataAccess
   DisplaysManager
   Shared
   WindowsAPI
   Qt6::Core
   Qt6::Gui
   Qt6::Widgets
)
