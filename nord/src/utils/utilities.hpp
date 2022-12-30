#pragma once

#include <iostream>
#include <string>  // std::string

#include "config/config.hpp"
#include "logger/logger.hpp"

namespace nord
{
    class utilities
    {
       public:
        static std::string format( const char *const format, va_list ap );

       private:
    };

    extern utilities util_mgr;
}  // namespace nord
