#include "offsets.hpp"

namespace nord::rbx
{
    void offsets::initialize()
    {
        print = process_hook_mgr.mem.proc->format( print );

        log_mgr.log_debug( "offsets", "Initialized offsets\n" );
    }

    offsets offsets_mgr;
}  // namespace nord::rbx