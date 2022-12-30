#include "utilities.hpp"

#include <cstdarg>    // va_start, va_end, std::va_list
#include <cstddef>    // std::size_t
#include <stdexcept>  // std::runtime_error
#include <vector>     // std::vector

namespace nord
{
    std::string utilities::format( const char *const format, va_list ap )
    {
        std::string message;
        va_list ap_copy;
        va_copy( ap_copy, ap );
        size_t len = vsnprintf( 0, 0, format, ap_copy );
        message.resize( len + 1 );  // need space for NUL
        vsnprintf( &message[ 0 ], len + 1, format, ap );
        message.resize( len );  // remove the NUL
        return message;
    }

    utilities util_mgr;
}  // namespace nord