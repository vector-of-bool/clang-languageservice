get_filename_component(zip_file "${TMP_DIR}/clang.zip" ABSOLUTE)
message(STATUS "Temporary zip file is ${zip_file}")
set(download YES)
if(EXISTS "${zip_file}")
    file(MD5 "${zip_file}" md5)
    if(md5 STREQUAL CLANG_MD5)
        set(download NO)
    endif()
endif()
if(download)
    file(DOWNLOAD
        ${CLANG_URL}
        "${zip_file}"
        SHOW_PROGRESS
        EXPECTED_MD5 ${CLANG_MD5}
        )
endif()

set(do_extract YES)

get_filename_component(dest ${SOURCE_DIR}/tools/clang ABSOLUTE)
if(EXISTS ${dest} AND EXISTS ${dest}/${LLVM_BRANCH}.stamp)
    set(do_extract NO)
endif()

if(do_extract)
    set(msg "Extracting clang source into LLVM source at ${SOURCE_DIR}...")
    message(STATUS "${msg}")
    execute_process(
        WORKING_DIRECTORY "${SOURCE_DIR}/tools"
        RESULT_VARIABLE res
        COMMAND "${CMAKE_COMMAND}" -E tar
            xf "${zip_file}"
    )
    file(REMOVE_RECURSE "${SOURCE_DIR}/tools/clang")
    file(RENAME "${SOURCE_DIR}/tools/clang-${LLVM_BRANCH}" "${SOURCE_DIR}/tools/clang")
    message(STATUS "${msg} - Done")
endif()