#pragma once

#include <time.h>

#include <memory>
#include <string>

#include "../offsets.hpp"

namespace nord::rbx
{
    enum class message_type
    {
        output,
        info,
        warning,
        error,
        sensitive, // doesn't show up in developer console
    };

    class standard_out
    {
       public:
        static std::shared_ptr< standard_out > get();

        void print( message_type type, const char *message );
        void print( message_type type, std::string message );
        void printf( message_type type, const char *format, ... );

       private:
        // prevent creation of the class outside of the singleton class
        standard_out()
        {
        }
    };

}  // namespace nord::rbx