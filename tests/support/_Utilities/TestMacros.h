#pragma once

#include <gtest/gtest.h>

#define TEST_GWT(Suite, Name, Given, When, Then) \
TEST(Suite, Name) { \
      SCOPED_TRACE(std::string("\nGIVEN: ") + Given + \
                   "\nWHEN:  " + When + \
                   "\nTHEN:  " + Then + "\n");

// Optional: Also create one for Test Fixtures (TEST_F)
#define TEST_F_GWT(Suite, Name, Given, When, Then) \
   TEST_F(Suite, Name) { \
         SCOPED_TRACE(std::string("\nGIVEN: ") + Given + \
                      "\nWHEN:  " + When + \
                      "\nTHEN:  " + Then + "\n");
