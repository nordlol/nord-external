#include "process_hook.hpp"

namespace nord
{
    bool process_hook::load_window()
    {
        if ( window = FindWindow( nullptr, L"Roblox" ) )
        {
            // Note: maybe make this a little nicer? dumb check
            if ( memory::get_client_rect( window, &screen ) )
                return window_active = window == GetForegroundWindow();
        }

        return false;
    }

    void process_hook::update()
    {
        while ( true )
        {
            load_window();
        }
    }

    void process_hook::start()
    {
        std::thread t( &process_hook::update, this );
        t.detach();
    }

    process_hook process_hook_mgr;
}  // namespace nord