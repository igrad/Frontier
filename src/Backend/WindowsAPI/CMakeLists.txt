set(WindowsAPISources
   WindowsAPI.cpp
   WindowsAPI.h
   WindowsAPIInterface.h
   WindowsAPISettings.h
   WindowsEventMessageFilter.cpp
   WindowsEventMessageFilter.h
)

add_library(WindowsAPI STATIC
   ${WindowsAPISources}
)

target_include_directories(WindowsAPI PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(WindowsAPI PRIVATE
   DisplaysManager
   Shared
   Qt6::Core
)
