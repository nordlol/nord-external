#include "workspace.hpp"

#include <execution>

#include "../../process_hook.hpp"

namespace nord::rbx
{
    camera workspace::current_camera() const
    {
        return process_hook_mgr.mem.proc->read< std::uintptr_t >( get_address() + 0x338 );
    }


}  // namespace nord::rbx