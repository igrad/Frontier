include(FetchContent)
FetchContent_Declare(
   googletest
   GIT_REPOSITORY https://github.com/google/googletest.git
   GIT_TAG v1.17.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

find_package(Qt6 REQUIRED COMPONENTS Multimedia MultimediaWidgets)

enable_testing()

qt_add_executable(FrontierUnitTests
   MANUAL_FINALIZATION
   ${CMAKE_SOURCE_DIR}/assets/fonts.qrc
   ${CMAKE_SOURCE_DIR}/assets/images.qrc
   main.cpp
)

target_link_libraries(FrontierUnitTests
   PRIVATE
   Qt6::Core
   Qt6::Test
   Qt6::Widgets
   Qt6::Multimedia
   Qt6::MultimediaWidgets
   GTest::gtest
   GTest::gmock
)

# mocks, fakes, etc
add_subdirectory(support)

# Test subdirs
add_subdirectory(Backend)
add_subdirectory(DataAccess)
add_subdirectory(UI)

include(GoogleTest)
gtest_discover_tests(FrontierUnitTests)

qt_finalize_executable(FrontierUnitTests)
