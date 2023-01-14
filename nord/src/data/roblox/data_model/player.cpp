#include "player.hpp"

#include "../../process_hook.hpp"

namespace nord::rbx
{
    rbx::instance player::character() const
    {
        if ( process_hook_mgr.data_model->game_id() != phantom_forces )
            return process_hook_mgr.mem.proc->read< std::uintptr_t >( get_address() + 0x6C );

        return *this;
    }

    rbx::instance player::team() const
    {
        const auto address = process_hook_mgr.mem.proc->read< std::uintptr_t >( get_address() + 0xA0 );

        if ( !address )
            return rbx::instance( 0 );

        return address;
    }

    bool player::is_teammate( player player ) const
    {
        return player.team().get_address() && team().get_address() && player.team() == team();
    }

    std::tuple< part, part > player::get_parts()
    {
        const auto head = character().get_child_by_name< part >( "Head" );
        const auto torso = character().get_child_by_name< part >( "HumanoidRootPart" );

        return std::make_tuple( head, torso );
    }

    bool player::is_body_part( const rbx::part& part )
    {
        return std::find( std::begin( regular_body_parts ), std::end( regular_body_parts ), part.name() ) !=
               std::end( regular_body_parts );
    }

}  // namespace nord::rbx