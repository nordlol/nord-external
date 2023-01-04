#include "player.hpp"

#include "../../process_hook.hpp"

namespace nord::rbx
{
    rbx::instance player::character() const
    {
        return process_hook_mgr.mem.proc->read< std::uintptr_t >( get_address() + 0x7C );
    }

    std::tuple< part, part > player::get_parts()
    {
        const auto head = character().get_child_by_name< part >( "Head" );
        const auto torso = character().get_child_by_name< part >( "HumanoidRootPart" );

        return std::make_tuple( head, torso );
    }

}  // namespace nord::rbx