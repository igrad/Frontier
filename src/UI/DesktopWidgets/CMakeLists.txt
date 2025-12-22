set(DesktopWidgetsSources

)

add_library(DesktopWidgets ${DesktopWidgetsSources})
target_include_directories(DesktopWidgets PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(DesktopWidgets PRIVATE
   Qt6::Core
   Qt6::Gui
   Qt6::Widgets
   Shared
   SharedUIComponents
)
