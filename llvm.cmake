include(ExternalProject)

set(LLVM_BRANCH release_39)

set(LLVM_URL "https://github.com/llvm-mirror/llvm/archive/${LLVM_BRANCH}.zip")
set(LLVM_MD5 c5750e5563b162013ae52b7dd0ad54d3)
set(CLANG_URL "https://github.com/llvm-mirror/clang/archive/${LLVM_BRANCH}.zip")
set(CLANG_MD5 1c19129ec64b48c0e371e34b3de4416c)

set(NULL_COMMAND ${CMAKE_COMMAND} -E echo_append)
ExternalProject_Add(
    LLVM
    EXCLUDE_FROM_ALL 1
    URL ${LLVM_URL}
    URL_HASH MD5=${LLVM_MD5}
    PATCH_COMMAND
        ${CMAKE_COMMAND}
            "-DLLVM_BRANCH=${LLVM_BRANCH}"
            "-DCLANG_URL=${CLANG_URL}"
            "-DCLANG_MD5=${CLANG_MD5}"
            "-DSOURCE_DIR=<SOURCE_DIR>"
            "-DBINARY_DIR=<BINARY_DIR>f"
            "-DTMP_DIR=<TMP_DIR>"
            -P "${CMAKE_CURRENT_LIST_DIR}/download-clang.cmake"
    CMAKE_ARGS
        -DLLVM_TARGETS_TO_BUILD:string=
        -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_LIST_DIR}/extern/llvm
    BUILD_COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR> --target clangTooling --config $<CONFIG>
    INSTALL_DIR "${CMAKE_CURRENT_LIST_DIR}/extern/llvm"
    # Make Ninja show output:
    USES_TERMINAL_DOWNLOAD 1
    USES_TERMINAL_PATCH 1
    USES_TERMINAL_CONFIGURE 1
    USES_TERMINAL_BUILD 1
)

list(INSERT CMAKE_PREFIX_PATH 0 "${CMAKE_CURRENT_LIST_DIR}/extern/llvm")
find_package(LLVM 3.9 REQUIRED)
find_package(Clang REQUIRED)
if(NOT TARGET libclang)
    message(WARNING "Failed to find libclang. You probably don't have it installed properly. Build the 'LLVM' target and reconfigure.")
else()
    add_library(clang::libclang INTERFACE IMPORTED)
    set_property(TARGET clang::libclang APPEND PROPERTY INTERFACE_LINK_LIBRARIES libclang)
    set_property(TARGET clang::libclang APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${LLVM_DIR}/../../../include")
endif()