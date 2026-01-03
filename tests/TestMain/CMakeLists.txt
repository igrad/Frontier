add_library(TestMain STATIC
   main.cpp
)
add_library(TestSupport::TestMain ALIAS TestMain)

target_link_libraries(TestMain
   PUBLIC
   Qt6::Core
   Qt6::Sql
   GTest::gtest
)

target_compile_features(TestMain PUBLIC cxx_std_17)
target_compile_definitions(TestMain PRIVATE UNIT_TESTING)

target_include_directories(TestMain PRIVATE
   ${CMAKE_SOURCE_DIR}/src
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_SOURCE_DIR}/../support
)
