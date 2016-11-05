project(libclangxx)
cmake_minimum_required(VERSION 2.8)

add_subdirectory(libclangxx)

include(CTest)

if(BUILD_TESTING AND PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    find_package(Boost)
    if(NOT DEFINED Boost_FOUND)
        message(SEND_ERROR "Could not find Boost to do unit testing. BUILD_TESTS will be disabled")
        set(BUILD_TESTS OFF)
    else()
        enable_testing()
        add_subdirectory(tests)
    endif()
endif()
