set(EnterpriseSources
   EnterpriseWindow.cpp
   EnterpriseWindow.h
   EnterpriseService.cpp
   EnterpriseService.h

   SettingsModel/EnterpriseSettingsModel.cpp
   SettingsModel/EnterpriseSettingsModel.h
   SettingsModel/RowData.h

   Tabs/HardwareTab/EnterpriseHardwareTab.cpp
   Tabs/HardwareTab/EnterpriseHardwareTab.h
   Tabs/HardwareTab/EnterpriseMonitorWidget.cpp
   Tabs/HardwareTab/EnterpriseMonitorWidget.h
   Tabs/HardwareTab/EnterpriseMonitorWindow.cpp
   Tabs/HardwareTab/EnterpriseMonitorWindow.h

   Tabs/SettingsTab/EnterpriseSettingsTab.cpp
   Tabs/SettingsTab/EnterpriseSettingsTab.h
   Tabs/SettingsTab/EnterpriseSettingsView.cpp
   Tabs/SettingsTab/EnterpriseSettingsView.h
)

add_library(Enterprise ${EnterpriseSources})
target_include_directories(Enterprise PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/SettingsModel
   ${CMAKE_CURRENT_SOURCE_DIR}/Tabs/HardwareTab
   ${CMAKE_CURRENT_SOURCE_DIR}/Tabs/SettingsTab
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
