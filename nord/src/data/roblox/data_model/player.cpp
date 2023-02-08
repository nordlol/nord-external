#include "player.hpp"

#include "../../process_hook.hpp"

namespace nord::rbx
{
    instance player::character() const
    {
        switch ( process_hook_mgr.data_model->game_id() )
        {
            case game_t::phantom_forces: return *this;
            case game_t::counter_blox: return process_hook_mgr.workspace.get_child_by_name( name() );
        }
        return process_hook_mgr.mem.proc->read< std::uintptr_t >( get_address() + 0x6C );
    }

    rbx::team player::team() const
    {
        if ( process_hook_mgr.data_model->game_id() == game_t::phantom_forces &&
             parent().get_address() != process_hook_mgr.players.get_address() )
        {
            const auto name = this->parent().name() == "Bright blue" ? "Phantoms" : "Ghosts";
            return process_hook_mgr.teams.get_child_by_name< rbx::team >( name );
        }

        const auto address = process_hook_mgr.mem.proc->read< std::uintptr_t >( get_address() + 0xA0 );

        if ( !address )
            return rbx::team( 0 );

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

    std::tuple< engine::vector2_t, engine::vector2_t > player::get_part_screen_locations()
    {
        const auto& [ head, torso ] = get_parts();

        engine::vector2_t head_pos{ -1, -1 };
        engine::vector2_t torso_pos{ -1, -1 };

        if ( !head.get_address() || !torso.get_address() )
            return std::make_tuple( head_pos, torso_pos );

        const auto head_screen = process_hook_mgr.visual_engine->world_to_screen( head.cfame().translation );
        const auto torso_screen = process_hook_mgr.visual_engine->world_to_screen( torso.cfame().translation );

        if ( !head_screen || !torso_screen )
            return std::make_tuple( head_pos, torso_pos );

        return std::make_tuple( *head_screen, *torso_screen );
    }

    part player::get_root_part() const
    {
        return character().get_child_by_name< part >( "HumanoidRootPart" );
    }

    bool player::is_body_part( const rbx::part& part )
    {
        return regular_body_parts.find( part.name() ) != regular_body_parts.end();
    }

}  // namespace nord::rbx