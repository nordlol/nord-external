#include "camera.hpp"

#include "../../process_hook.hpp"

namespace nord::rbx
{
    engine::cframe_t camera::cframe() const
    {
        return process_hook_mgr.mem.proc->read< engine::cframe_t >( get_address() + 0x78 );
    }

    std::int32_t camera::get_distance_from( part part ) const
    {
        return ( cframe().translation - part.cfame().translation ).magnitude();
    }
}  // namespace nord::rbx