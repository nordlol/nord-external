#include "memory.hpp"

#define CALC_RECT

namespace nord
{
    bool memory::get_client_rect( HWND handle, screen_t* screen )
    {
        POINT point{};
        RECT rect{};

        if ( ClientToScreen( handle, &point ) && GetClientRect( handle, &rect ) )
        {
#ifndef CALC_RECT
            *screen = { point.x, point.y, 500, 500 };
#else
            *screen = { point.x, point.y, rect.right - rect.left, rect.bottom - rect.top };
#endif  // !USE_CSHARP

            return true;
        }

        return false;
    }
}  // namespace nord