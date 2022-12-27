#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <windows.h>

namespace nord
{
    enum class log_level
    {
        debug,
        warning,
        error
    };

    class logger final
    {
       public:
        logger()
        {
            const auto file_name = "./logs/" + time_str( get_local_time(), "yyyy-MM-dd") + ".txt";
            log_file = std::ofstream{ file_name };
            console = IsDebuggerPresent();
        }

        void close()
        {
            log_file.close();
        }

        template< typename... Args >
        void log_debug( const char* const source, Args... args )
        {
            log( log_level::debug, source, args... );
        }

        template< typename... Args >
        void log_error( const char* const source, Args... args )
        {
            log( log_level::error, source, args... );
        }

       private:
        std::tm* get_local_time();
        std::string time_str( std::tm* tm, const char* const format );
        const char* log_level_str( log_level level );

        template< typename T >
        void MagicLog( std::ostream& o, T t )
        {
            o << t;
        }

        template< typename T, typename... Args >
        void MagicLog( std::ostream& o, T t, Args... args )  // recursive variadic function
        {
            MagicLog( o, t );
            MagicLog( o, args... );
        }

        template< typename... Args >
        void log( log_level level, const char* const source, Args... args )
        {
            std::ostringstream out;
            out << time_str( get_local_time(), "%T" ) << " [" << log_level_str( level ) << "] " << source << ": ";
            MagicLog( out, args... );

            std::cout << out.str();
        }
        
        std::ofstream log_file;
        bool console;
    };

    extern logger log_mgr;
}  // namespace nord