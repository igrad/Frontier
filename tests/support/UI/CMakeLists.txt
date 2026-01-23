set(UISupportSources
   CoreUI/ShellWindowMock.cpp
   CoreUI/ShellWindowMock.h
)

add_library(UISupport
   ${UISupportSources}
)
add_library(TestSupport::UISupport ALIAS UISupport)

target_include_directories(UISupport
   PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/CoreUI
   ${CMAKE_SOURCE_DIR}/UI
)

target_link_libraries(UISupport
   PUBLIC
   AssetManager
   DisplaysManager
   CoreUI
   Qt6::Core
   Qt6::Widgets
   GTest::gmock
)
