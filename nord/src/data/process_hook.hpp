#pragma once

#include <Windows.h>

#include <iostream>
#include <thread>

#include "../memory/memory.hpp"
#include "../utils/logger/logger.hpp"

namespace nord
{
    class process_hook final
    {
       public:
        process_hook( const wchar_t* const process_name, const wchar_t* const window_name )
            : window_name( window_name ),
              mem( process_name )
        {
        }

        // starts the process hook.
        bool start();
        bool destroy();

        HWND window = nullptr;
        screen_t screen{};
        memory mem;
        bool window_active = false;

        bool load_window();

       private:
        // load roblox classes
        bool load_roblox();

        const wchar_t* const window_name;
    };

    extern process_hook process_hook_mgr;
}  // namespace nord
