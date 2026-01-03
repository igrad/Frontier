set(TestUtilitiesSources
   FakeSettingSubscriber.h
   TestMacros.h
)

add_library(TestUtilities
   ${TestUtilitiesSources}
)
add_library(TestSupport::TestUtilities ALIAS TestUtilities)

target_include_directories(TestUtilities
   PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(TestUtilities
   PUBLIC
   Qt6::Core
)
