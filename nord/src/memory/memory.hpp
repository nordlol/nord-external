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
    } screen_t;

    class memory
    {
       public:
        static bool get_client_rect( HWND handle, screen_t* screen );

       private:
    };

}  // namespace nord