cmake_minimum_required(VERSION 3.4.0)
project(clang-languageservice VERSION 0.0.0)

include(CTest)
enable_testing()

include_directories(extern)

include(llvm.cmake)

if(NOT TARGET clang::libclang)
    message(WARNING "Cannot continue to build without libclang")
    return()
endif()

add_subdirectory(extern/clangxx)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED system thread)
add_library(nlohmann::json INTERFACE IMPORTED)
set_property(TARGET nlohmann::json APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/extern/nlohmann-json/src")
add_library(Boost::di INTERFACE IMPORTED)
set_property(TARGET Boost::di APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/extern/boost-di/include")

add_executable(${PROJECT_NAME}
    main.cpp
    language_service.cpp
    json_rpc/windows_stdin_stream.cpp
    )
target_compile_features(${PROJECT_NAME} PRIVATE cxx_auto_type)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::thread Boost::system nlohmann::json Boost::disable_autolinking Boost::di clang::libclang libclangxx)
target_compile_definitions(${PROJECT_NAME} PRIVATE BOOST_THREAD_VERSION=4)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)