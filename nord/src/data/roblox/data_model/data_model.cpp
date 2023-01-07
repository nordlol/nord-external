#include "data_model.hpp"

#include "../../process_hook.hpp"

namespace nord::rbx
{
    std::int64_t data_model::game_id() const
    {
        return process_hook_mgr.mem.proc->read< std::uintptr_t >( get_address() - 12 + 0x3F0 );
    }
}