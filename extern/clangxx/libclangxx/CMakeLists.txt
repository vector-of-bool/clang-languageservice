# Public headers!
set(libclangxx_PUBLIC_HEADERS
    utils.hpp
    cxx1y_fix.hpp
    index.hpp
    translation_unit.hpp
    cursor.hpp
    string_utils.hpp
    location.hpp
    unsaved_file.hpp
    file.hpp
	errors.hpp
    )

# Add a dummy lirary for exporting. It won't have any symbol in it.
add_library(libclangxx INTERFACE)

# Set some properties that will be exported:
set_target_properties(libclangxx PROPERTIES INTERFACE_INCLUDE_DIRECTORIES $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>)

# Make this target available to the testing sources
export(TARGETS libclangxx FILE ${CMAKE_BINARY_DIR}/exports/libclangxx.targets.cmake)

# The install work is done in here:
include(install.cmake)
