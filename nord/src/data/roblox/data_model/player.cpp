#include "player.hpp"

#include "../../process_hook.hpp"

namespace nord::rbx
{
    rbx::instance player::character() const
    {
        return process_hook_mgr.mem.proc->read< std::uintptr_t >( get_address() + 0x7C );
    }
}  // namespace nord::rbx