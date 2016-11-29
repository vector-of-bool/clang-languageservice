file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/types.in" types_lines)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/types.in")

set(output_lines
    "#ifndef CLS_TYPES_HPP_GENERATED_INCLUDED"
    "#define CLS_TYPES_HPP_GENERATED_INCLUDED"
    ""
    "#include <map>"
    "#include <string>"
    "#include <vector>"
    ""
    "#include <boost/optional.hpp>"
    ""
    "#include <json.hpp>"
    "#include <mirror/mirror.hpp>"
    ""
    )

set(current_namespace)
set(current_interface)
set(current_fields)

macro(cls_new_namespace)
    list(APPEND output_lines
        "namespace ${current_namespace} {"
        "   using std::string{{sc}}"
        "   using std::vector{{sc}}"
        "   using std::map{{sc}}"
        "   using nlohmann::json{{sc}}"
        "   using boost::optional{{sc}}"
        "}"
        "")
endmacro()

macro(cls_finish_struct)
    list(APPEND output_lines
        "namespace ${current_namespace} { struct ${current_interface} {"
        )
    foreach(field IN LISTS current_fields)
        list(APPEND output_lines "    ${field}{{sc}}")
    endforeach()
    list(APPEND output_lines
        "}{{sc}} }"
        ""
        "MIRRORPP_REFLECT(${current_namespace}::${current_interface},"
        )
    foreach(field IN LISTS current_fields)
        if(field MATCHES ".*[ \t]([A-Za-z1-9]+)$")
            list(APPEND output_lines "                (${CMAKE_MATCH_1})")
        else()
            message(SEND_ERROR "Mal-formed field: ${field}")
        endif()
    endforeach()
    list(APPEND output_lines "                ){{sc}}" "")
endmacro()

foreach(line_ws IN LISTS types_lines)
    string(STRIP "${line_ws}" line)
    if(NOT line MATCHES "^#" # Skip comments
        AND NOT line STREQUAL "" # Skip blank lines
    )
        if(line MATCHES "^namespace[ \t]+(.*)")
            string(MAKE_C_IDENTIFIER "${CMAKE_MATCH_1}" new_ns)
            if(current_interface)
                cls_finish_struct()
            endif()
            set(current_interface)
            set(current_namespace "${new_ns}")
            cls_new_namespace()
        elseif(line MATCHES "^interface[ \t]+(.*)")
            string(MAKE_C_IDENTIFIER "${CMAKE_MATCH_1}" new_iface)
            if(current_interface)
                cls_finish_struct()
            endif()
            set(current_fields)
            set(current_interface "${new_iface}")
        else()
            list(APPEND current_fields "${line}")
        endif()
    endif()
endforeach()

if(current_interface)
    cls_finish_struct()
endif()

list(APPEND output_lines "" "#endif")

string(REPLACE ";" "\n" types_hpp_content "${output_lines}")
string(REPLACE "{{sc}}" ";" types_hpp_content "${types_hpp_content}")
file(WRITE "types.hpp" "${types_hpp_content}")

add_library(langsrv STATIC
    language_service.hpp
    language_service.cpp
    protocol_types.hpp

    compilation_database.hpp

    # Individual methods
    cls_rename.cpp
    )
target_link_libraries(langsrv PUBLIC jsonrpc clang::libTooling)