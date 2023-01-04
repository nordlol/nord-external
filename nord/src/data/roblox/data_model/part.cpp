#include "part.hpp"

#include "../../process_hook.hpp"

namespace nord::rbx
{
    std::uintptr_t part::primitive() const
    {
        return process_hook_mgr.mem.proc->read< std::uintptr_t >( get_address() + 0xC8 );
    }

    engine::cframe_t part::cfame() const
    {
        return process_hook_mgr.mem.proc->read< engine::cframe_t >( primitive() + 0xD8 );
    }

    engine::vector3_t part::size() const
    {
        return process_hook_mgr.mem.proc->read< engine::vector3_t >( primitive() + 0x1E0 );
    }
}  // namespace nord::rbx