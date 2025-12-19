#pragma once

#include <gtest/gtest.h>

#define GWT(Given, When, Then) \
SCOPED_TRACE(std::string("\n===GIVEN: ") + Given + \
                "\n===WHEN:  " + When + \
                "\n===THEN:  " + Then + "\n");

#define WT(When, Then) \
SCOPED_TRACE(std::string("\n===WHEN:  ") + When + \
                "\n===THEN:  " + Then + "\n");
