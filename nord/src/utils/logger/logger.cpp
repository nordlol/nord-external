#define _CRT_SECURE_NO_WARNINGS

#include "logger.hpp"

#include <chrono>
#include <ctime>
#include <iomanip>


namespace nord
{
    // thank you stack overflow
    std::tm *logger::get_local_time()
    {
        using namespace std::chrono;

        // get current time
        auto now = system_clock::now();

        // get number of milliseconds for the current second
        // (remainder after division into seconds)
        auto ms = duration_cast< milliseconds >( now.time_since_epoch() ) % 1000;

        // convert to std::time_t in order to convert to std::tm (broken time)
        auto timer = system_clock::to_time_t( now );

        // convert to broken time
        return std::localtime( &timer );
    }

    std::string logger::time_str( std::tm* tm, const char* const format )
    {
        std::ostringstream oss;
        oss << std::put_time( tm, format );
        return oss.str();
    }

    const char* logger::log_level_str( log_level level )
    {
        switch ( level )
        {
            case nord::log_level::debug: return "debug";
            case nord::log_level::warning: return "warning";
            case nord::log_level::error: return "error";
            default: break;
        }
    }

    logger log_mgr{};
}  // namespace nord