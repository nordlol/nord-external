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
    nord::log_mgr.log_debug( "main()", "Waiting for target process\n" );

    while ( !nord::process_hook_mgr.window_active )
        std::this_thread::sleep_for( std::chrono::microseconds( 10 ) );

    // start overlay
    if (!nord::overlay_mgr.start())
        nord::log_mgr.log_error( "main()", "Unable to start the overlay\n" );

    nord::log_mgr.close();
}