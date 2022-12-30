#include "standard_out.hpp"

#include <format>

namespace nord::rbx
{
    std::shared_ptr< standard_out > standard_out::get()
    {
        static std::shared_ptr< standard_out > sout( new standard_out() );
        return sout;
    }

    void standard_out::print( message_type type, const char *message )
    {
        auto function = process_hook_mgr.mem.proc->create_sub< int >( offsets_mgr.print );
        function( ( int )type, message );
    }

    void standard_out::printf( message_type type, const char *format, ... )
    {
        va_list arg_list;
        va_start( arg_list, format );
        std::string message = utilities::format( format, arg_list );
        va_end( arg_list );
        print( type, message.c_str() );
    }

    void standard_out::print( message_type type, std::string message )
    {
        print( type, message.c_str() );
    }

}  // namespace nord::rbx