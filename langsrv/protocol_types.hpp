#ifndef LANGSRV_PROTOCOL_TYPES_HPP_INCLUDED
#define LANGSRV_PROTOCOL_TYPES_HPP_INCLUDED

#include "types.hpp"

namespace langsrv {

enum class MessageType {
    Error = 1,
    Warning = 2,
    Info = 3,
    Log = 4,
};

}

#endif // LANGSRV_PROTOCOL_TYPES_HPP_INCLUDED