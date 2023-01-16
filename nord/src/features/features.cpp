#include "features.hpp"

#include "../overlay/menu/menu.hpp"

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
        if ( config_mgr.get< bool >( "fov_circle" ) )
        {
            rbx::engine::vector2_t screen = process_hook_mgr.visual_engine->viewport();

            overlay_mgr.render_list.add< render::circle >(
                ImVec2{ screen.x / 2, screen.y / 2 },
                config_mgr.get< float >( "fov_size" ),
                config_mgr.get< ImColor >( "fov_color" ) );
        }

        if ( config_mgr.get< bool >( "player_esp" ) )
        {
            const auto local_player = process_hook_mgr.players.local_player();

            for ( auto& player : process_hook_mgr.players.get_children_as< rbx::player >() )
            {
                if ( player.get_address() == local_player.get_address() )
                    continue;

                // printf( "%x %x\n", player.team().get_address(), local_player.team().get_address() );
                // printf( "%d\n", player.is_teammate( local_player ) );
                if ( config_mgr.get< bool >( "team_check" ) && player.is_teammate( local_player ) )
                    continue;

                ImColor color = get_player_color( player, player.is_teammate( local_player ) );

                const auto character = player.character();

                if ( !character.get_address() )
                    continue;

                const auto distance = process_hook_mgr.camera.get_distance_from( player.get_root_part() );

                if ( config_mgr.get< bool >( "distance_check" ) && distance >= config_mgr.get< int >( "render_distance" ) )
                    continue;

                // Specating or other weird stuff
                if ( distance <= 10 )
                    continue;

                if ( config_mgr.get< bool >( "box_esp_dynamic" ) )
                    dynamic_box_esp( player, color, distance );
                else
                    static_box_esp( player, color, distance );
            }
        }
    }

    ImColor features::get_player_color( rbx::player player, bool teammate )
    {
        if ( config_mgr.get< bool >( "use_team_color" ) )
            return player.team().team_color().color();

        return teammate ? config_mgr.get< ImColor >( "ally_color" ) : config_mgr.get< ImColor >( "enemy_color" );
    }

    void features::static_box_esp( rbx::player player, ImColor color, std::int32_t distance )
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

        name_esp( torso_x, torso_y - height - 15, player, distance );

        overlay_mgr.render_list.add< render::rectangle >(
            ImVec2{ torso_x - width, torso_y + height }, ImVec2{ torso_x + width, torso_y - height }, color );
    }

    void features::dynamic_box_esp( rbx::player player, ImColor color, std::int32_t distance )
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

        name_esp( ( max_x - min_x ) / 2 + min_x, min_y - 15, player, distance );

        overlay_mgr.render_list.add< render::rectangle >( ImVec2{ max_x, max_y }, ImVec2{ min_x, min_y }, color );
    }

    void features::name_esp( float x, float y, rbx::player player, std::int32_t distance )
    {
        if ( config_mgr.get< bool >( "name_esp" ) )
        {
            const auto size = config_mgr.get< bool >( "autoscale_names" ) ? clamp_distance( distance, 12 ) : 0.0f;

            overlay_mgr.render_list.add< render::text >(
                ImVec2{ x, y }, size, ImColor{ 255, 255, 255 }, player.name(), true );
        }
    }

    float features::clamp_distance( std::int32_t distance, std::int32_t clamp )
    {
        const auto size = 1 / distance * 1000;

        if ( size < 0 )
            return 0;

        if ( size > clamp )
            return clamp;

        return size;
    }

    features feature_mgr;
}  // namespace nord