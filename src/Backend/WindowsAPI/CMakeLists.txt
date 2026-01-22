set(WindowsAPISources
   WindowsAPI.cpp
   WindowsAPI.h
   WindowsAPIInterface.h
   WindowsAPISettings.h
   WindowsEventMessageFilter.cpp
   WindowsEventMessageFilter.h

   Win32APIWrapper/Win32APIWrapper.cpp
   Win32APIWrapper/Win32APIWrapper.h
   Win32APIWrapper/Win32APIWrapperFake.cpp
   Win32APIWrapper/Win32APIWrapperFake.h
   Win32APIWrapper/Win32APIWrapperInterface.h
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
