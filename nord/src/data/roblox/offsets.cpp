#include "offsets.hpp"

namespace nord::rbx
{
    bool offsets::initialize()
    {
        print = process_hook_mgr.mem.proc->format( 0x10358F0 );

        if ( !init_get_task_scheduler() )
            return false;

        log_mgr.log_debug( "offsets", "Successfully initialized offsets\n" );
        return true;
    }

    bool offsets::init_get_task_scheduler()
    {
        if ( !( get_task_scheduler = process_hook_mgr.mem.proc->sig_scan(
                    "\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x08\xE8\x00\x00\x00\x00\x8D\x0C\x24", "xxxxxxxxxx????xxx" ) ) )
        {
            log_mgr.log_error( "offsets", "Failed to locate RBX::TaskScheduler signature\n" );
            return false;
        }

        // we have found the signature, so trace the relative call
        get_task_scheduler =
            ( get_task_scheduler + 14 ) + process_hook_mgr.mem.proc->read< std::uintptr_t >( get_task_scheduler + 10 );

        log_mgr.log_debug( "offsets", "Located RBX::TaskScheduler::singleton() at 0x%x\n", get_task_scheduler );
        return true;
    }

    offsets offsets_mgr;
}  // namespace nord::rbx