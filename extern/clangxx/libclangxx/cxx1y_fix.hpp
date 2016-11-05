#ifndef LIBCLANGXX_CXX1Y_FIX_HPP
#define LIBCLANGXX_CXX1Y_FIX_HPP

/// Implements std::gets, which was removed from C11. Clang has an issue
/// parsing cstdio with C++1y support enabled. #including this function
/// declaration fixes it.
// extern "C" char* gets( char* );

#endif // LIBCLANGXX_CXX1Y_FIX_HPP
