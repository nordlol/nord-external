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

        // load roblox specific classes
        if ( !load_roblox() )
        {
            nord::log_mgr.log_error( "process_hook", "Failed to load roblox specific classes\n" );
            return false;
        }

        return true;
    }

    bool process_hook::load_roblox()
    {
        // init offsets as we have a handle and module
        if ( !rbx::offsets_mgr.initialize() )
        {
            log_mgr.log_error( "process_hook", "Failed to initialize offsets\n" );
            return false;
        }

        // local, won't need it globally
        std::shared_ptr< rbx::task_scheduler > scheduler = nullptr;

        rbx::standard_out::get()->printf( rbx::message_type::info, "Loaded process_hook sucessfully" );

        if ( !( scheduler = rbx::task_scheduler::get() ) )
        {
            log_mgr.log_error( "process_hook", "Failed to retrieve task scheduler instance\n" );
            return false;
        }

        log_mgr.log_debug( "process_hook", "Located RBX::TaskScheduler instance at 0x%x\n", scheduler->get_address() );

        if ( !scheduler->set_frame_delay( config_mgr.get< std::int32_t >( "fps" ) ) )
            log_mgr.log_warning(
                "process_hook",
                "Unable to locate frame delay offset in RBX::TaskScheduler, could be set by external process\n" );

        if ( !( data_model = scheduler->get_data_model() ) )
        {
            log_mgr.log_error( "process_hook", "Failed to get RBX::DataModel from task scheduler\n" );
            return false;
        }

        log_mgr.log_debug( "process_hook", "Retrieved RBX::DataModel instance at 0x%x\n", data_model->get_address() );

        if ( !( visual_engine = scheduler->get_visual_engine() ) )
        {
            log_mgr.log_error( "process_hook", "Failed to get RBX::Graphics::VisualEngine from task scheduler\n" );
            return false;
        }

        log_mgr.log_debug(
            "process_hook", "Retrieved RBX::Graphics::VisualEngine instance at 0x%x\n", visual_engine->get_address() );

        players = data_model->get_child_by_class_name( "Players" );
        return true;
    }

    process_hook process_hook_mgr{ L"RobloxPlayerBeta.exe", L"Roblox" };
}  // namespace nord