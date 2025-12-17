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

# mocks, fakes, etc
add_subdirectory(support)

add_subdirectory(Backend)

include(GoogleTest)
gtest_discover_tests(FrontierUnitTests)
