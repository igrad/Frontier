set(OverlaysSources

)

add_library(Overlays ${OverlaysSources})
target_include_directories(Overlays PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Overlays PRIVATE
   Qt6::Core
   Qt6::Gui
   Qt6::Widgets
   Shared
   SharedUIComponents
)
