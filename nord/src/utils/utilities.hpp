#pragma once

#include <iostream>

#include "logger/logger.hpp"

namespace nord
{
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
