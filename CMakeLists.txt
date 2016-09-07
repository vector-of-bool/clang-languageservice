cmake_minimum_required(VERSION 3.4.0)
project(clang-languageservice VERSION 0.0.0)

include(CTest)
enable_testing()

find_package(Boost REQUIRED system)
add_library(nlohmann::json INTERFACE IMPORTED)
set_property(TARGET nlohmann::json APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/extern/nlohmann-json/src")

add_executable(${PROJECT_NAME} main.cpp)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_auto_type)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::system nlohmann::json)
target_compile_definitions(${PROJECT_NAME} PRIVATE BOOST_THREAD_VERSION=3)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)