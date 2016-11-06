include(ExternalProject)

set(NULL_COMMAND ${CMAKE_COMMAND} -E echo_append)

set(boost_extern_dir "${CMAKE_CURRENT_LIST_DIR}/extern/boost")

ExternalProject_Add(Boost
    EXCLUDE_FROM_ALL 1
    URL https://sourceforge.net/projects/boost/files/boost/1.62.0/boost_1_62_0.tar.bz2
    URL_HASH SHA256=36c96b0f6155c98404091d8ceb48319a28279ca0333fba1ad8611eb90afb2ca0
    CONFIGURE_COMMAND
        ${CMAKE_COMMAND} -E chdir <SOURCE_DIR>
        <SOURCE_DIR>/bootstrap.sh --prefix=<INSTALL_DIR> --with-libraries=system,thread
    BUILD_COMMAND
        ${CMAKE_COMMAND} -E chdir <SOURCE_DIR>
        <SOURCE_DIR>/b2 -j4
    INSTALL_COMMAND
        ${CMAKE_COMMAND} -E chdir <SOURCE_DIR>
        <SOURCE_DIR>/b2 install headers
    INSTALL_DIR "${boost_extern_dir}"
    # Make Ninja show output:
    USES_TERMINAL_DOWNLOAD 1
    USES_TERMINAL_PATCH 1
    USES_TERMINAL_CONFIGURE 1
    USES_TERMINAL_BUILD 1
    USES_TERMINAL_INSTALL 1
    )

set(Boost_USE_STATIC_LIBS ON)
foreach(root IN ITEMS "${boost_extern_dir}")
    set(BOOST_ROOT "${root}")
    find_package(Boost 1.62 COMPONENTS system thread)
    if(Boost_FOUND)
        break()
    endif()
endforeach()
set(have_boost NO)
if(Boost_FOUND)
    if(TARGET Boost::thread AND TARGET Boost::system)
        set(have_boost YES)
    else()
        set(got_threadlib NO)
        get_filename_component(threadlib "${boost_extern_dir}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}boost_thread${CMAKE_STATIC_LIBRARY_SUFFIX}" ABSOLUTE)
        message(STATUS "Searching for in-source Boost.Thread library ${threadlib}")
        if(EXISTS "${threadlib}")
            add_library(Boost::thread STATIC IMPORTED)
            set_target_properties(Boost::thread PROPERTIES
                IMPORTED_LOCATION "${threadlib}"
                INTERFACE_INCLUDE_DIRECTORIES "${boost_extern_dir}/include"
                )
            set(got_threadlib YES)
        endif()
        get_filename_component(syslib "${boost_extern_dir}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}boost_system${CMAKE_STATIC_LIBRARY_SUFFIX}" ABSOLUTE)
        message(STATUS "Searching for in-source Boost.System library ${syslib}")
        if(EXISTS "${syslib}")
            add_library(Boost::system STATIC IMPORTED)
            set_target_properties(Boost::system PROPERTIES
                IMPORTED_LOCATION "${syslib}"
                INTERFACE_INCLUDE_DIRECTORIES "${boost_extern_dir}/include"
                )
            set(got_syslib YES)
        endif()
        if(got_threadlib AND got_syslib)
            set(have_boost YES)
        endif()
    endif()
endif()

if(NOT have_boost)
    message(WARNING "Boost libraries not found. Build the 'Boost' target and reconfigure.")
endif()