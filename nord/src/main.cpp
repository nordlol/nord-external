#include <iostream>
#include <thread>

#include "data/process_hook.hpp"
#include "overlay/overlay.hpp"

std::int32_t main()
{
    // start our process hook
    nord::process_hook_mgr.start();

    // start overlay
    nord::overlay_mgr.start();

    std::cin.get();
}