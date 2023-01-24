#include "primitive.hpp"

#include "../../process_hook.hpp"

namespace nord::rbx
{
    engine::cframe_t primitive::cfame() const
    {
        return process_hook_mgr.mem.proc->read< engine::cframe_t >( get_address() + 0xD8 );
    }

    engine::vector3_t primitive::size() const
    {
        return process_hook_mgr.mem.proc->read< engine::vector3_t >( get_address() + 0x1E0 );
    }

    float primitive::radius() const
    {
        return 0.5f * size().magnitude();
    }
}  // namespace nord::rbx