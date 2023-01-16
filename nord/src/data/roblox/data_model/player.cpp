#include "player.hpp"

#include "../../process_hook.hpp"

namespace nord::rbx
{
    rbx::instance player::character() const
    {
        if ( process_hook_mgr.data_model->game_id() != game_t::phantom_forces )
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
        const auto torso = get_root_part();

        return std::make_tuple( head, torso );
    }

    part player::get_root_part()
    {
        return character().get_child_by_name< part >( "HumanoidRootPart" );
    }

    bool player::is_body_part( const rbx::part& part )
    {
        return regular_body_parts.find( part.name() ) != regular_body_parts.end();
    }

}  // namespace nord::rbx