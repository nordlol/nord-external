#include "team.hpp"

#include "../../process_hook.hpp"

namespace nord::rbx
{
    brick_color team::team_color()
    {
        return process_hook_mgr.mem.proc->read< brick_color::brick_number >( get_address() + 0x60 );
    }
}  // namespace nord::rbx