cmake_minimum_required(VERSION 3.16)
project(Frontier VERSION 0.1 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Packages
find_package(Qt6 REQUIRED COMPONENTS Core Gui Sql Widgets)

# Global Includes (So tests and app can find Shared headers)
include_directories(src/Shared)

# Process Subdirectories
# 1. src defines CoreUI, Shared, and the Frontier executable
add_subdirectory(src)

# 2. tests defines FrontierUnitTests and links to the libs created in src
enable_testing()
add_subdirectory(tests)
