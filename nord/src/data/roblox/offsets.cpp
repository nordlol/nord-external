#include "offsets.hpp"

#include "../process_hook.hpp"

namespace nord::rbx
{
    std::shared_ptr< offsets > offsets::get()
    {
        static std::shared_ptr< offsets > instance( new offsets() );
        return instance;
    }

    void offsets::initialize()
    {
        print = rebase( print );

        log_mgr.log_debug( "offsets", "Initialized offsets\n" );
    }

    constexpr std::uintptr_t offsets::rebase( std::uintptr_t address )
    {
        return process_hook_mgr.mem.module.base + address - 0x400000;
    }

}  // namespace nord::rbx