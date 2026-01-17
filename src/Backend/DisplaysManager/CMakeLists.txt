set(DisplaysManagerSources
   DisplayInfo.h
   DisplaysManager.cpp
   DisplaysManager.h
   DisplaysManagerInterface.h
)

add_library(DisplaysManager ${DisplaysManagerSources})
target_include_directories(DisplaysManager PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_SOURCE_DIR}/src
   ${CMAKE_SOURCE_DIR}/src/DataAccess
)

target_link_libraries(DisplaysManager PUBLIC
   DataAccess
   Shared
   WindowsAPI
   PRIVATE
   Qt6::Core
)
