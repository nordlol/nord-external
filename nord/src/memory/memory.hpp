#pragma once

#include <Windows.h>

#include <iostream>
#include <optional>

#include "../utils/utilities.hpp"

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

    typedef std::uintptr_t( __stdcall* callback )( void* data );

    class memory
    {
       public:
        memory( const wchar_t* const name ) : name( name )
        {
        }

        bool start();
        bool destroy();

        // read process memory
        template< typename T >
        inline T read( std::uintptr_t address )
        {
            T buffer;
            if ( !ReadProcessMemory( handle, reinterpret_cast< LPCVOID >( address ), &buffer, sizeof( T ), nullptr ) )
            {
                log_mgr.log_error( "memory", "Error when attempting to read from 0x%x", address );
                return buffer;
            }
        }

        // write process memory
        template< typename T >
        inline T write( std::uintptr_t address, T buffer )
        {
            return write< T >( reinterpret_cast< LPCVOID >( address ), buffer );
        }

        template< typename T >
        inline T write( LPCVOID address, T buffer )
        {
            if ( !WriteProcessMemory( handle, address, &buffer, sizeof( T ), nullptr ) )
            {
                log_mgr.log_error( "memory", "Error when attempting to write to 0x%x", address );
                return buffer;
            }
        }

        std::uintptr_t find_pattern( const char* const pattern, std::uintptr_t size = 0 );

        // static methods
        static bool get_client_rect( HWND handle, screen_t* screen );

        module_t module{ 0 };
        HANDLE handle = nullptr;

        void* CreateCharPointerString( const char* string );

       private:
        HANDLE get_process();
        std::optional< module_t > get_module( HANDLE handle, const wchar_t* module_name );

        bool check_pattern( const std::uint8_t* data, const char* pattern );

        const wchar_t* const name;
    };

    template< class RetType >
    class function
    {
       public:
        function( memory* mem, const std::uintptr_t address ) : mem( mem ), address( address )
        {
        }

       private:
        memory* mem;
        std::uintptr_t address;
    };

}  // namespace nord