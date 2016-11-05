# Copyright (c) 2014, Colby Pike
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of LibClang++ nor the
#       names of its contributors may be used to endorse or promote products
#       derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL COLBY PIKE BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

option(ALWAYS_BUILD_TESTS "Build tests as part of the default target" ON)

find_package(Boost REQUIRED COMPONENTS unit_test_framework)

include(${CMAKE_BINARY_DIR}/exports/libclangxx.targets.cmake)

function(configure_test target)
    message(STATUS "Configuring test ${target}")
    target_include_directories(${target} BEFORE PUBLIC ${CMAKE_SOURCE_DIR})
    target_link_libraries(${target} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} libclangxx)
    set(binary $<TARGET_FILE:${target}>)
    add_test(NAME ${target} COMMAND $<TARGET_FILE:${target}>)
endfunction()

set(TEST_SOURCES
    basic.cpp
    parsing.cpp
    translation_unit.cpp
    errors.cpp
    cursors.cpp
    )

set(TEST_FILES
    test_cxx.cpp
    test_cxx11.cpp
    test_cxx1y.cpp
    )

foreach(source ${TEST_SOURCES})
    add_executable(${source} ${source} testing_header.hpp)
    configure_test(${source})
endforeach()

add_custom_target(copy-test-files ALL COMMENT "Copying test files"
    SOURCES ${TEST_FILES}
    )

foreach(file ${TEST_FILES})
    set(path ${CMAKE_CURRENT_SOURCE_DIR}/${file})
    add_custom_command(TARGET copy-test-files
        COMMAND ${CMAKE_COMMAND} -E copy ${path} ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Copying ${file} to the testing directory"
        )
endforeach()
