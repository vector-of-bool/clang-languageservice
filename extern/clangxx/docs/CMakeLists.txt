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

find_program(DOXYGEN_PROG doxygen DOC "The path to the Doxygen executable")

if(${DOXYGEN_PROG} STREQUAL DOXYGEN_PROG-NOTFOUND)
    message(STATUS "Couldn't find doxygen to generate documentation")
else()
    message(STATUS "Found doxygen: ${DOXYGEN_PROG}")
    message(STATUS "Build the 'docs' target to automatically generate documentation")

    set(doc_SOURCES
        docs.hpp
        gs_parsing.cpp
        ex_intercepting.cpp
        ex_finding.cpp
        )
    add_custom_target(docs
        COMMAND cd ${CMAKE_SOURCE_DIR} && ${DOXYGEN_PROG}
        COMMENT "Generating documentation"
        SOURCES ${doc_SOURCES}
        )
    if(NOT TARGET build-doc-examples)
        add_custom_target(build-doc-examples)
    endif()
    foreach(source ${doc_SOURCES})
        if(NOT ${source} MATCHES ".*\\.hpp")
            add_executable(_${source}-ex EXCLUDE_FROM_ALL ${source})
            target_link_libraries(_${source}-ex libclangxx)
            set_target_properties(_${source}-ex PROPERTIES COMPILE_FLAGS -std=c++1y)
            add_dependencies(build-doc-examples _${source}-ex)
        endif()
    endforeach()
endif()
