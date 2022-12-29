#pragma once

#include <time.h>

#include <string>
#include <memory>

#include "../offsets.hpp"

namespace nord::rbx
{
    enum class message_type
    {
        output,
        info,
        warning,
        error,
        sensitive,
    };

    class standard_out
    {
       public:
        static std::shared_ptr< standard_out > get();


        void print( message_type type, const char *message );
        void printf( message_type type, const char *format, ... );
       private:
        // prevent creation of the class outside of the singleton class
        standard_out()
        {
        }
    };

}  // namespace nord::rbx