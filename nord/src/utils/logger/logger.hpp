#pragma once

#include <windows.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

namespace nord
{
    namespace fs = std::filesystem;

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
            const fs::path logs{ "logs" };
            const fs::path full_path = fs::current_path() / logs;

            const auto file_name = full_path / ( time_str( get_local_time(), "yyyy-MM-dd" ) + ".txt" );

            // create directory if logs don't exists
            fs::create_directory( full_path );

            log_file = std::ofstream{ file_name };

            console = IsDebuggerPresent();
        }

        void close()
        {
            log_file.close();
        }

        template< typename... Args >
        void log_debug( const char* const source, const char* const format, Args... args )
        {
            log( log_level::debug, source, format, args... );
        }

        template< typename... Args >
        void log_error( const char* const source, const char* const format, Args... args )
        {
            log( log_level::error, source, format, args... );
        }

        template< typename... Args >
        void log_warning( const char* const source, const char* const format, Args... args )
        {
            log( log_level::warning, source, format, args... );
        }

       private:
        std::tm* get_local_time();
        std::string time_str( std::tm* tm, const char* const format );
        const char* log_level_str( log_level level );

        template< typename... Args >
        void log( log_level level, const char* const source, const char* const format, const auto&... args )
        {
            std::ostringstream out;
            out << time_str( get_local_time(), "%T" ) << " [" << log_level_str( level ) << "] " << source << ": " << format;

            if ( console )
                std::printf( out.str().c_str(), args... );
            log_file << out.str();
        }

        std::ofstream log_file;
        bool console;
    };

    extern logger log_mgr;
}  // namespace nord