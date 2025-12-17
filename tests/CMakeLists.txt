add_subdirectory(Backend)
include(FetchContent)
FetchContent_Declare(
   googletest
   URL "github.com/google/googletest/releases/tag/v1.17.0"
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

add_executable(FrontierUnitTests "")

target_link_libraries(FrontierUnitTests PRIVATE
   gtest_main
   Qt6::Core
   Qt6::Test
)

gtest_discover_tests(FrontierUnitTests)
