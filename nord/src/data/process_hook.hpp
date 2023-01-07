#pragma once

#include <Windows.h>

#include <iostream>
#include <memory>
#include <thread>

#include "../memory/memory.hpp"
#include "../utils/logger/logger.hpp"
#include "roblox/data_model/players.hpp"
#include "roblox/scheduler/task_scheduler.hpp"

namespace nord
{
    typedef enum games : std::uint64_t
    {
        phantom_forces = 113491250,
        bad_buisness = 1168263273,
        rush_point = 2162282815,
        recoil = 1534453623,
        apocalypse_rising_2 = 358276974
    } games_t;

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

        // roblox classes
        std::shared_ptr< rbx::data_model > data_model = nullptr;
        std::shared_ptr< rbx::visual_engine > visual_engine = nullptr;

        rbx::players players;

       private:
        // load roblox classes
        bool load_roblox();

        const wchar_t* const window_name;
    };

    extern process_hook process_hook_mgr;
}  // namespace nord
