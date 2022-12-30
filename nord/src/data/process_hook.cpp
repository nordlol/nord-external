#include "process_hook.hpp"
#include "roblox/util/standard_out.hpp"

namespace nord
{
    bool process_hook::load_window()
    {
        if ( window = FindWindow( nullptr, window_name ) )
        {
            // Note: maybe make this a little nicer? dumb check
            if ( memory::get_client_rect( window, &screen ) )
                return window_active = window == GetForegroundWindow();
        }

        return false;
    }

    bool process_hook::start()
    {
        // wait for the target window to come up
        nord::log_mgr.log_debug( "process_hook", "Waiting for target process\n" );

        while ( !load_window() )
            std::this_thread::sleep_for( std::chrono::microseconds( 10 ) );

        // target process has been found
        if ( !mem.proc->success() )
        {
            nord::log_mgr.log_error( "process_hook", "Failed to locate process handle or module\n" );
            return false;
        }

        // init offsets as we have a handle and module
        rbx::offsets_mgr.initialize();

        rbx::standard_out::get()->printf( rbx::message_type::warning, "This is a warning.");

        return true;
    }

    bool process_hook::destroy()
    {
        return true;
    }

    process_hook process_hook_mgr{ L"RobloxPlayerBeta.exe", L"Roblox" };
}  // namespace nord