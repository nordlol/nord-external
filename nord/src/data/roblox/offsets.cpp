#include "offsets.hpp"

#include "../process_hook.hpp"

namespace nord::rbx
{
    bool offsets::initialize()
    {
        if ( !init_print() )
            return false;

        if ( !init_get_task_scheduler() )
            return false;

        if ( !init_get_health() )
            return false;

        // if ( !init_get_max_health() )
        //    return false;

        log_mgr.log_debug( "offsets", "Successfully initialized offsets\n" );
        return true;
    }

    bool offsets::init_get_task_scheduler()
    {
        if ( !( get_task_scheduler = process_hook_mgr.mem.proc->sig_scan(
                    "\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x08\xE8\x00\x00\x00\x00\x8D\x0C\x24", "xxxxxxxxxx????xxx" ) ) )
        {
            log_mgr.log_error( "offsets", "Failed to locate RBX::TaskScheduler::singleton() signature\n" );
            return false;
        }

        // we have found the signature, so trace the relative call
        get_task_scheduler =
            ( get_task_scheduler + 14 ) + process_hook_mgr.mem.proc->read< std::uintptr_t >( get_task_scheduler + 10 );

        log_mgr.log_debug( "offsets", "Located RBX::TaskScheduler::singleton() at 0x%x\n", get_task_scheduler );
        return true;
    }

    bool offsets::init_print()
    {
        if ( !( print = process_hook_mgr.mem.proc->sig_scan(
                    "\x55\x8B\xEC\x6A\xFF\x68\xCC\xCC\xCC\xCC\x64\xA1\x00\x00\x00\x00\x50\x64\x89\x25\x00\x00\x00\x00\x83"
                    "\xEC\x1C\x8B\x55\x0C\x8D",
                    "xxxxxx????xxxxxxxxxxxxxxxxxxxxx" ) ) )
        {
            log_mgr.log_error( "offsets", "Failed to locate RBX::StandardOut::print() signature\n" );
            return false;
        }

        log_mgr.log_debug( "offsets", "Located RBX::StandardOut::print() at 0x%x\n", print );
        return true;
    }

    bool offsets::init_get_health()
    {
        if ( !( get_health = process_hook_mgr.mem.proc->sig_scan( "\x55\x8B\xEC\x51\x8B\x81\x14", "xxxxxxx" ) ) )
        {
            log_mgr.log_error( "offsets", "Failed to locate RBX::Humanoid::getHealth() signature\n" );
            return false;
        }

        // get_health = process_hook_mgr.mem.proc->format( 0x8F0D80 );
        log_mgr.log_debug( "offsets", "Located RBX::Humanoid::getHealth() at 0x%x\n", get_health );
        return true;
    }

    bool offsets::init_get_max_health()
    {
        if ( !( get_max_health =
                    process_hook_mgr.mem.proc->sig_scan( "\x55\x8B\xEC\x51\x8B\x81\x18\x02\x00\x00\x8B", "xxxxxxxxxxx" ) ) )
        {
            log_mgr.log_error( "offsets", "Failed to locate RBX::Humanoid::getMaxHealth() signature\n" );
            return false;
        }

        log_mgr.log_debug( "offsets", "Located RBX::Humanoid::getMaxHealth() at 0x%x\n", get_max_health );
        return true;
    }

    offsets offsets_mgr;
}  // namespace nord::rbx