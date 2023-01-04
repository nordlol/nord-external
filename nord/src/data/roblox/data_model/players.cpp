#include "players.hpp"

#include "../../process_hook.hpp"

namespace nord::rbx
{
    player players::local_player() const
    {
        return process_hook_mgr.mem.proc->read< std::uintptr_t >( get_address() + 0x140 );
    }
}