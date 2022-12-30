#pragma once

#include <Windows.h>

#include <iostream>
#include <optional>

#include "../utils/utilities.hpp"
#include "glad-ext.hpp"

#define in_range( x, a, b ) ( x >= a && x <= b )
#define get_bits( x )       ( in_range( x, '0', '9' ) ? ( x - '0' ) : ( ( x & ( ~0x20 ) ) - 'A' + 0xa ) )
#define get_byte( x )       ( get_bits( x[ 0 ] ) << 4 | get_bits( x[ 1 ] ) )

namespace nord
{
    typedef struct screen
    {
        std::int32_t x;
        std::int32_t y;
        std::int32_t width;
        std::int32_t height;

        bool operator==( const screen& screen )
        {
            return x == screen.x && y == screen.y && width == screen.width && height == screen.height;
        }

        bool operator!=( const screen& screen )
        {
            return !( *this == screen );
        }
    } screen_t;

    typedef struct module
    {
        std::uintptr_t base;
        std::uintptr_t size;
    } module_t;

    class memory
    {
       public:
        memory( const wchar_t* const name ) : name( name )
        {
            proc = std::make_unique< xg_process >( name );
        }

        // static methods
        static bool get_client_rect( HWND handle, screen_t* screen );

        std::unique_ptr< xg_process > proc = nullptr;
       private:
        const wchar_t* const name;
    };

}  // namespace nord