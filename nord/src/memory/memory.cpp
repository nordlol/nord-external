#include "memory.hpp"

namespace nord
{
    bool memory::get_client_rect( HWND handle, screen_t* screen )
    {
        POINT point{};
        RECT rect{};

        if ( ClientToScreen( handle, &point ) && GetClientRect( handle, &rect ) )
        {
            *screen = { point.x, point.y, rect.right - rect.left, rect.bottom - rect.top };
            return true;
        }

        return false;
    }
}  // namespace nord