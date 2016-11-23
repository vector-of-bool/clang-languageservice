include(ExternalProject)

set(NULL_COMMAND ${CMAKE_COMMAND} -E echo_append)

set(boost_extern_dir "${CMAKE_CURRENT_LIST_DIR}/extern/boost")

if(WIN32)
    set(bootstrap_ext bat)
else()
    set(bootstrap_ext sh)
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(address_model 64)
else()
    set(address_model 32)
endif()

set(b2_args
    --prefix=<INSTALL_DIR>
    --with-system
    --with-thread
    --with-atomic
    --with-date_time
    -j4
    address-model=${address_model}
    link=static
    variant=$<$<CONFIG:Debug>:debug>$<$<NOT:$<CONFIG:Debug>>:release>
    threading=multi
    )

ExternalProject_Add(Boost
    EXCLUDE_FROM_ALL 1
    URL https://sourceforge.net/projects/boost/files/boost/1.62.0/boost_1_62_0.tar.bz2
    URL_HASH SHA256=36c96b0f6155c98404091d8ceb48319a28279ca0333fba1ad8611eb90afb2ca0
    CONFIGURE_COMMAND
        ${CMAKE_COMMAND} -E chdir <SOURCE_DIR>
        # ${CMAKE_COMMAND} -E env
        #     CXX=${CMAKE_CXX_COMPILER}
        #     CC=${CMAKE_C_COMPILER}
        <SOURCE_DIR>/bootstrap.${bootstrap_ext}
    BUILD_COMMAND
        ${CMAKE_COMMAND} -E chdir <SOURCE_DIR>
        # ${CMAKE_COMMAND} -E env
        #     CXX=${CMAKE_CXX_COMPILER}
        #     CC=${CMAKE_C_COMPILER}
        <SOURCE_DIR>/b2 ${boost_args} ${b2_args}
    INSTALL_COMMAND
        ${CMAKE_COMMAND} -E chdir <SOURCE_DIR>
        # ${CMAKE_COMMAND} -E env
        #     CXX=${CMAKE_CXX_COMPILER}
        #     CC=${CMAKE_C_COMPILER}
        <SOURCE_DIR>/b2 -j4 ${boost_args} ${b2_args} install headers
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
    find_package(Boost 1.62 COMPONENTS system thread atomic chrono date_time)
    if(Boost_FOUND)
        break()
    endif()
endforeach()
set(have_boost NO)
if(Boost_FOUND)
    if(TARGET Boost::thread AND TARGET Boost::system)
        set(have_boost YES)
    else()
        if(NOT TARGET Boost::boost)
            add_library(Boost::boost INTERFACE IMPORTED)
            set_target_properties(Boost::boost PROPERTIES
                INTERFACE_INCLUDE_DIRECTORIES "${boost_extern_dir}/include"
                )
        endif()
        set(got_threadlib NO)
        get_filename_component(threadlib "${boost_extern_dir}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}boost_thread${CMAKE_STATIC_LIBRARY_SUFFIX}" ABSOLUTE)
        message(STATUS "Searching for in-source Boost.Thread library ${threadlib}")
        if(EXISTS "${threadlib}")
            add_library(Boost::thread STATIC IMPORTED)
            set_target_properties(Boost::thread PROPERTIES
                IMPORTED_LOCATION "${threadlib}"
                INTERFACE_LINK_LIBRARIES Boost::boost
                )
            set(got_threadlib YES)
        endif()
        get_filename_component(syslib "${boost_extern_dir}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}boost_system${CMAKE_STATIC_LIBRARY_SUFFIX}" ABSOLUTE)
        message(STATUS "Searching for in-source Boost.System library ${syslib}")
        if(EXISTS "${syslib}")
            add_library(Boost::system STATIC IMPORTED)
            set_target_properties(Boost::system PROPERTIES
                IMPORTED_LOCATION "${syslib}"
                INTERFACE_LINK_LIBRARIES Boost::boost
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