set(SharedSources
   ArgParser.cpp
   ArgParser.h

   DisplayInfo.h

   Log.h
   Logging/Log.cpp
   Logging/Logger.cpp
   Logging/Logger.h

   Time/Timer.cpp
   Time/Timer.h

   Utilities/EnumHelpers.h
   Utilities/MethodLookupHelpers.h
   Utilities/Rando.cpp
   Utilities/Rando.h
   Utilities/XPtr.h

   Sql/SqlQuery.h
)

add_library(Shared ${SharedSources})
target_include_directories(Shared PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Shared PRIVATE
   Qt6::Core
   Qt6::Sql
)
