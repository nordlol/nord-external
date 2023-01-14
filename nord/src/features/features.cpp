#include "features.hpp"

namespace nord
{
    void features::start()
    {
        std::thread visuals(
            [ & ]()
            {
                while ( true )
                {
                    run_visuals();

                    overlay_mgr.render_list.update();
                }
            } );

        visuals.detach();
    }

    void features::run_visuals()
    {
        if ( config_mgr.get< bool >( "esp" ) )
        {
            const auto local_player = process_hook_mgr.players.local_player();

            for ( auto& player : process_hook_mgr.players.get_children_as< rbx::player >() )
            {
                if ( player.get_address() == local_player.get_address() )
                    continue;

                if ( config_mgr.get< bool >( "team_check" ) && player.is_teammate( local_player ) )
                    continue;

                const auto character = player.character();

                if ( !character.get_address() )
                    continue;

                if ( config_mgr.get< bool >( "box_esp_dynamic" ) )
                    dynamic_box_esp( player );
                else
                    static_box_esp( player );
            }
        }
    }

    void features::static_box_esp( rbx::player player )
    {
        const auto [ head, root ] = player.get_parts();

        if ( !head.get_address() || !root.get_address() )
            return;

        const auto head_location = process_hook_mgr.visual_engine->world_to_screen(
            root.cfame().translation + rbx::engine::vector3_t( 0, 0.9f, 0 ) );

        const auto torso_location = process_hook_mgr.visual_engine->world_to_screen(
            root.cfame().translation - rbx::engine::vector3_t( 0, 0.4f, 0 ) );

        if ( !head_location && !torso_location )
            return;

        const auto [ head_x, head_y ] = head_location.value();
        const auto [ torso_x, torso_y ] = torso_location.value();

        const auto width = std::fabsf( head_y - torso_y );
        const auto height = std::fabsf( head_y - torso_y ) * 2.2f;

        name_esp( torso_x, torso_y - height - 15, player );

        overlay_mgr.render_list.add< render::rectangle >(
            ImVec2{ torso_x - width, torso_y + height },
            ImVec2{ torso_x + width, torso_y - height },
            ImColor{ 255, 255, 255 } );
    }

    void features::dynamic_box_esp( rbx::player player )
    {
        std::vector< rbx::engine::vector3_t > all_corners;

        for ( auto& part : player.character().get_children_as< rbx::part >() )
        {
            if ( !player.is_body_part( part ) )
                continue;

            auto corners = rbx::visual_engine::get_corners( part.cfame().translation, part.size() );

            all_corners.insert( all_corners.end(), corners.begin(), corners.end() );
        }

        auto [ min_x, min_y ] = process_hook_mgr.visual_engine->viewport();
        float max_x = 0.0f, max_y = 0.0f;

        for ( const auto& corner : all_corners )
        {
            const auto part_position = process_hook_mgr.visual_engine->world_to_screen( corner );

            if ( !part_position )
                return;

            if ( part_position.value().x > max_x )
                max_x = part_position.value().x;

            if ( part_position.value().y > max_y )
                max_y = part_position.value().y;

            if ( part_position.value().x < min_x )
                min_x = part_position.value().x;

            if ( part_position.value().y < min_y )
                min_y = part_position.value().y;
        }

        name_esp( ( max_x - min_x ) / 2 + min_x, min_y - 15, player );

        overlay_mgr.render_list.add< render::rectangle >(
            ImVec2{ max_x, max_y }, ImVec2{ min_x, min_y }, ImColor{ 255, 255, 255 } );
    }

    void features::name_esp( float x, float y, rbx::player player )
    {
        if ( config_mgr.get< bool >( "name_esp" ) )
        {
            overlay_mgr.render_list.add< render::text >( ImVec2{ x, y }, ImColor{ 255, 255, 255 }, player.name(), true );
        }
    }

    features feature_mgr;
}  // namespace nord