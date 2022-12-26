#pragma once

#include <Windows.h>

#include <iostream>

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
            return !(*this == screen);
        }
    } screen_t;

    class memory
    {
       public:
        static bool get_client_rect( HWND handle, screen_t* screen );

       private:
    };

}  // namespace nord