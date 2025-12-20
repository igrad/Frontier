target_sources(FrontierUnitTests PRIVATE
   TestMacros.h
   FakeSettingSubscriber.h
)

target_include_directories(FrontierUnitTests PRIVATE
   ${CMAKE_CURRENT_SOURCE_DIR}
)
