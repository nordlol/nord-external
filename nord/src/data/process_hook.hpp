#pragma once

#include <Windows.h>

#include <iostream>
#include <thread>

#include "../memory/memory.hpp"

namespace nord
{
    class process_hook final
    {
       public:
        /// Starts the process hook.
        /// </summary>
        //void start();

        HWND window;
        screen_t screen;
        bool window_active = false;

        bool load_window();

       private:
        void update();
        
    };

    extern process_hook process_hook_mgr;
}  // namespace nord
