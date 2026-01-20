target_sources(FrontierUnitTests
   PRIVATE
   TaskBarServiceTests.cpp
   TaskBarServiceWorkerTests.cpp
   TaskBarSettingsProxyTests.cpp
)

target_include_directories(FrontierUnitTests
   PRIVATE
   ${CMAKE_SOURCE_DIR}/Backend/TaskBarService
)

target_link_libraries(FrontierUnitTests
   PUBLIC
   TaskBarService
   TestSupport::DataAccessSupport
   TestSupport::TestUtilities
   TestSupport::BackendSupport
)
