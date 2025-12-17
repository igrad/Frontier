#pragma once

#include <gtest/gtest.h>

#define GWT(Given, When, Then) \
SCOPED_TRACE(std::string("\nGIVEN: ") + Given + \
                "\nWHEN:  " + When + \
                "\nTHEN:  " + Then + "\n");\
