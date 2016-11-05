message(STATUS "Installing to ${CMAKE_INSTALL_PREFIX}")

# Install the export targets
install(TARGETS libclangxx
    EXPORT libclangxx-install-targets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include/libclangxx
    )

# Export the install targets
install(EXPORT libclangxx-install-targets
    DESTINATION lib/LibClangXX
    FILE libclangxx-targets.cmake
    )

# Export config data:
include(CMakePackageConfigHelpers)

configure_package_config_file(
    libclangxx-config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/libclangxx-config.cmake"
    INSTALL_DESTINATION lib/libclangxx
    )

# Install the find_package file of magic:
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libclangxx-config.cmake"
    DESTINATION lib/LibClangXX
    )

# Be kind, rewind!
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake"
    IMMEDIATE @ONLY
    )
add_custom_target(uninstall
    "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake"
    )

