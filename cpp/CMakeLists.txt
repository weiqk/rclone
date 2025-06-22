cmake_minimum_required(VERSION 3.10)
project(RcloneCpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find libcurl
find_package(CURL REQUIRED)

# Add an executable
add_executable(rclone_cpp src/main.cpp src/network_manager.cpp)

# Link libcurl
target_link_libraries(rclone_cpp PRIVATE CURL::libcurl)

# Include directories
target_include_directories(rclone_cpp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Enable C++ exceptions
set_property(TARGET rclone_cpp PROPERTY CXX_STANDARD_FEATURES cxx_std_17_throw_exceptions)

# Optional: Add compiler warnings (GCC/Clang)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(rclone_cpp PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Print a message
message(STATUS "Configuring RcloneCpp project")
message(STATUS "CURL_FOUND: ${CURL_FOUND}")
message(STATUS "CURL_INCLUDE_DIRS: ${CURL_INCLUDE_DIRS}")
message(STATUS "CURL_LIBRARIES: ${CURL_LIBRARIES}")
