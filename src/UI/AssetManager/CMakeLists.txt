set(AssetManagerSources
   AssetClient.cpp
   AssetClient.h
   AssetManager.cpp
   AssetManager.h
)

add_library(AssetManager STATIC
   ${AssetManagerSources}
)

target_include_directories(AssetManager PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_SOURCE_DIR}/assets
   ${CMAKE_SOURCE_DIR}/src/DataAccess
   ${CMAKE_SOURCE_DIR}/src/DataAccess/AssetLoader
   ${CMAKE_SOURCE_DIR}/src/Shared
)

target_link_libraries(AssetManager PRIVATE
   DataAccess
   Shared
   Qt6::Core
   Qt6::Widgets
)
