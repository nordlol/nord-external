#include <iostream>
#include <thread>

#include "data/process_hook.hpp"
#include "features/features.hpp"
#include "overlay/overlay.hpp"
#include "utils/utilities.hpp"

std::int32_t main()
{
    if ( !nord::process_hook_mgr.start() )
        nord::log_mgr.log_error( "main()", "Unable to start process hook\n" );

    nord::feature_mgr.start();

    // start overlay
    if ( !nord::overlay_mgr.start() )
        nord::log_mgr.log_error( "main()", "Unable to start the overlay\n" );

    nord::log_mgr.close();
}