set(EnterpriseSources
   EnterpriseWindow.cpp
   EnterpriseWindow.h
   EnterpriseService.cpp
   EnterpriseService.h
   EnterpriseSettingsModel.cpp
   EnterpriseSettingsModel.h
   EnterpriseSettingsView.cpp
   EnterpriseSettingsView.h

   RowData.h
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
   Qt6::Core
   Qt6::Gui
   Qt6::Widgets
   Shared
)
