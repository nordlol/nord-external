#pragma once

#include <iostream>

namespace nord
{
    enum class log_level
    {
        debug,
        warning,
        error
    };

    class utilities
    {
       public:
        void log( const char *const format, const auto &...args )
        {
            std::printf( format, args... );
        }

       private:
    };

    extern utilities util_mgr;
}  // namespace nord
