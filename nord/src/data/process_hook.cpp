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
            // if we have an invalid window (we are unable to extract the data we need), then invalidate the window so that
            // other
            // processes don't try and use faulty data.
            if ( !load_window() )
            {
                window = nullptr;
                screen = { 0, 0, 0, 0 };
                window_active = false;
            }

            if ( window_active )
                std::cout << "[DEBUG] screen { " << screen.x << ", " << screen.y << ", " << screen.width << ", "
                          << screen.height << " }\n";
        }
    }

    void process_hook::start()
    {
        std::thread t( &process_hook::update, this );
        t.detach();
    }

    process_hook process_hook_mgr;
}  // namespace nord