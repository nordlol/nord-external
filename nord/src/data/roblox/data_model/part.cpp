#include "part.hpp"

#include "../../process_hook.hpp"

namespace nord::rbx
{
    primitive part::primitive() const
    {
        return process_hook_mgr.mem.proc->read< std::uintptr_t >( get_address() + 0xB8 );
    }

    engine::cframe_t part::cfame() const
    {
        return primitive().cfame();
    }

    engine::vector3_t part::size() const
    {
        return primitive().size();
    }
}  // namespace nord::rbx