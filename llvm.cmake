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
        -DENABLE_SHARED=FALSE
        -DBUILD_SHARED=FALSE
        -DLLVM_TARGETS_TO_BUILD:string=
        -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_LIST_DIR}/extern/llvm
    BUILD_COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR> --target clangTooling --config $<CONFIG>
    INSTALL_COMMAND ${NULL_COMMAND}
    # Make Ninja show output:
    USES_TERMINAL_DOWNLOAD 1
    USES_TERMINAL_PATCH 1
    USES_TERMINAL_CONFIGURE 1
    USES_TERMINAL_BUILD 1
    USES_TERMINAL_INSTALL 1
)

list(INSERT CMAKE_PREFIX_PATH 0 "${CMAKE_CURRENT_LIST_DIR}/extern/llvm")
list(INSERT CMAKE_PREFIX_PATH 0 "${CMAKE_CURRENT_BINARY_DIR}/LLVM-prefix/src/LLVM-build")
find_package(LLVM 3.9)
if(LLVM_FOUND)
    find_package(Clang)
endif()

if(NOT TARGET clangTooling OR NOT LLVM_FOUND)
    message(WARNING "Failed to find LibTooling. You probably don't have it installed properly. Build the 'LLVM' target and reconfigure.")
else()
    add_library(clang::libTooling INTERFACE IMPORTED)
    set_property(TARGET clang::libTooling APPEND PROPERTY INTERFACE_LINK_LIBRARIES clangTooling)
    set_property(TARGET clang::libTooling
        APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
            "${LLVM_DIR}/../../../include"
            # "${CMAKE_CURRENT_BINARY_DIR}/LLVM-prefix/src/LLVM/include"
            # "${CMAKE_CURRENT_BINARY_DIR}/LLVM-prefix/src/LLVM/tools/clang/include"
        )
endif()