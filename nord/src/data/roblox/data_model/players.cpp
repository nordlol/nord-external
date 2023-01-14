#include "players.hpp"

#include "../../process_hook.hpp"

namespace nord::rbx
{
    player players::local_player() const
    {
        return process_hook_mgr.mem.proc->read< std::uintptr_t >( get_address() + 0x130 );
    }

    std::vector< instance > players::get_children() const
    {
        if ( process_hook_mgr.data_model->game_id() != phantom_forces )
            return instance::get_children();

        rbx::instance folder = process_hook_mgr.workspace.get_child_by_name< rbx::instance >( "Players" );

        return folder.get_child_by_name( local_player().team().name() == "Ghosts" ? "Bright blue" : "Bright orange" )
            .get_children();
    }
}  // namespace nord::rbx