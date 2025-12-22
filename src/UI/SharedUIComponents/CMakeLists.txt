set(SharedUIComponentsSources
   Widgets/AppWindow.cpp
   Widgets/AppWindow.h
   Widgets/Overlay.cpp
   Widgets/Overlay.h
)

add_library(SharedUIComponents ${SharedUIComponentsSources})
target_include_directories(SharedUIComponents PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(SharedUIComponents PRIVATE
   Qt6::Core
   Qt6::Gui
   Qt6::Widgets
   Shared
)
