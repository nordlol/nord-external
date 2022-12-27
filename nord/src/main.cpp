#include <iostream>
#include <thread>

#include "data/process_hook.hpp"
#include "overlay/overlay.hpp"
#include "utils/utilities.hpp"

std::int32_t main()
{
    // wait for the target window to come up
    nord::log_mgr.log_debug( "main()", "Waiting for target process\n" );

    while ( !nord::process_hook_mgr.window_active )
        nord::process_hook_mgr.load_window();

    // start overlay
    if (!nord::overlay_mgr.start())
        nord::log_mgr.log_error( "main()", "Unable to start the overlay\n" );

    nord::log_mgr.close();
}