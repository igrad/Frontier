include(FetchContent)
FetchContent_Declare(
   googletest
   GIT_REPOSITORY https://github.com/google/googletest.git
   GIT_TAG v1.17.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(FrontierUnitTests "")

target_link_libraries(FrontierUnitTests PRIVATE
   CoreUI
   Shared
   SettingsService
   gtest_main
   gmock_main
   Qt6::Core
   Qt6::Sql
)

target_compile_definitions(FrontierUnitTests PRIVATE UNIT_TESTING)

target_include_directories(FrontierUnitTests PRIVATE
   ${CMAKE_SOURCE_DIR}/src
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/support
)

# mocks, fakes, etc
add_subdirectory(support)

add_subdirectory(Backend)

include(GoogleTest)
gtest_discover_tests(FrontierUnitTests)
