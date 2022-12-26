#include <iostream>
#include <thread>

#include "data/process_hook.hpp"
#include "overlay/overlay.hpp"
#include "utils/utilities.hpp"

std::int32_t main()
{
    // start our process hook
    nord::process_hook_mgr.start();

    // wait for the target window to come up
    nord::util_mgr.log( "Waiting for target process...\n" );
    while ( !nord::process_hook_mgr.window_active )
        std::this_thread::sleep_for( std::chrono::microseconds( 10 ) );

    // start overlay
    nord::overlay_mgr.start();
}