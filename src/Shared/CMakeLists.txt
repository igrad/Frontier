set(SharedSources
   ArgParser.cpp
   ArgParser.h
   Log.h
   Logging/Log.cpp
   Logging/Logger.cpp
   Logging/Logger.h
)

add_library(Shared ${SharedSources})
target_include_directories(Shared PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Shared PRIVATE
   Qt6::Core
)
