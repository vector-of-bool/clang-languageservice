cmake_minimum_required(VERSION 3.4.0)
project(clang-languageservice VERSION 0.0.0)

include(CTest)
enable_testing()

include(CMakeToolsHelpers OPTIONAL)

include(boost.cmake)
include(llvm.cmake)

set(missing_deps NO)
if(NOT TARGET Boost::system OR NOT TARGET Boost::thread)
    message(WARNING "Cannot continue without Boost.System and Boost.Thread")
    set(missing_deps YES)
endif()

if(NOT TARGET clang::libTooling)
    message(WARNING "Cannot continue without libclang")
    set(missing_deps YES)
endif()

if(missing_deps)
    message(WARNING "Cannot configure without missing dependencies")
    return()
endif()

add_library(mirror::mirror INTERFACE IMPORTED)
set_target_properties(mirror::mirror PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/extern"
    INTERFACE_COMPILE_FEATURES cxx_auto_type
    )

add_library(nlohmann::json INTERFACE IMPORTED)
set_target_properties(nlohmann::json PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/extern/nlohmann-json/src"
    INTERFACE_COMPILE_FEATURES cxx_auto_type
    )

if(WIN32)
    set(stdin_stream_cpp json_rpc/windows_stdin_stream.cpp)
endif()

add_library(jsonrpc INTERFACE)
target_include_directories(jsonrpc INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)
target_link_libraries(jsonrpc INTERFACE nlohmann::json Boost::boost mirror::mirror)
target_compile_definitions(jsonrpc INTERFACE BOOST_THREAD_VERSION=4)

add_subdirectory(langsrv)

add_executable(${PROJECT_NAME}
    main.cpp
    )
target_compile_features(${PROJECT_NAME} PRIVATE cxx_auto_type)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        langsrv
        Boost::thread
        Boost::system
        nlohmann::json
        Boost::disable_autolinking
        clang::libTooling
    )
target_include_directories(${PROJECT_NAME} PRIVATE extern/)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)