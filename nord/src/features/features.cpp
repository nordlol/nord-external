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
        if ( config_mgr.get< bool >( "player_esp" ) )
        {
            const auto local_player = process_hook_mgr.players.local_player();

            for ( const auto& player : process_hook_mgr.players.get_children_as< rbx::player >() )
            {
                if ( player.get_address() == local_player.get_address() )
                    continue;

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
                if ( distance <= 5 )
                    continue;

                if ( config_mgr.get< bool >( "box_esp_dynamic" ) )
                    dynamic_box_esp( player, color, distance );
                else
                    static_box_esp( player, color, distance );
            }
        }

        draw_fov_circle();
    }

    void features::draw_fov_circle()
    {
        if ( config_mgr.get< bool >( "fov_circle" ) )
        {
            const auto fov = config_mgr.get< float >( "fov_size" );

            ImVec2 pos = get_fov_center();

            overlay_mgr.render_list.add< render::circle >( pos, fov, config_mgr.get< ImColor >( "fov_color" ) );
        }
    }

    void features::draw_snapline( rbx::engine::vector2_t pos, const ImVec2& head, const ImVec2& torso )
    {
        const auto& [ x, y ] = process_hook_mgr.visual_engine->viewport();
        const auto fov = config_mgr.get< float >( "fov_size" );

        if ( config_mgr.get< bool >( "snaplines" ) )
        {
            switch ( config_mgr.get< settings_types::aim_target >( "aim_target" ) )
            {
                case settings_types::aim_target::head:
                {
                    if ( pos.distance( { head.x, head.y } ) < fov )
                        overlay_mgr.render_list.add< render::line >(
                            ImVec2{ pos.x, pos.y }, head, config_mgr.get< ImColor >( "snaplines_color" ) );
                    break;
                }
                case settings_types::aim_target::torso:
                {
                    if ( pos.distance( { torso.x, torso.y } ) < fov )
                        overlay_mgr.render_list.add< render::line >(
                            ImVec2{ pos.x, pos.y }, torso, config_mgr.get< ImColor >( "snaplines_color" ) );
                    break;
                }
            }
        }
    }

    ImColor features::get_player_color( rbx::player player, bool teammate )
    {
        if ( config_mgr.get< bool >( "use_team_color" ) )
            return player.team().team_color().color();

        return teammate ? config_mgr.get< ImColor >( "ally_color" ) : config_mgr.get< ImColor >( "enemy_color" );
    }

    rbx::engine::vector2_t features::get_fov_center()
    {
        const auto& [ x, y ] = process_hook_mgr.visual_engine->viewport();

        ImVec2 pos{};

        switch ( config_mgr.get< settings_types::fov_type >( "fov_type" ) )
        {
            case settings_types::fov_type::center:
            {
                return { x / 2.0f, y / 2.0f };
            }
            case settings_types::fov_type::cursor:
            {
                return process_hook_mgr.get_mouse_position();
            }
        }

        return { -1, -1 };
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

        const auto [ head_x, head_y ] = *head_location;
        const auto [ torso_x, torso_y ] = *torso_location;

        const auto width = std::fabsf( head_y - torso_y );
        const auto height = std::fabsf( head_y - torso_y ) * 2.2f;

        if ( std::isnan( width ) || std::isnan( height ) )
            return;

        name_esp( torso_x, torso_y - height - 15, player, distance );
        draw_snapline( get_fov_center(), ImVec2{ head_x, head_y }, ImVec2{ torso_x, torso_y } );

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

            if ( ( *part_position ).x > max_x )
                max_x = ( *part_position ).x;

            if ( ( *part_position ).y > max_y )
                max_y = ( *part_position ).y;

            if ( ( *part_position ).x < min_x )
                min_x = ( *part_position ).x;

            if ( ( *part_position ).y < min_y )
                min_y = ( *part_position ).y;
        }

        name_esp( ( max_x - min_x ) / 2 + min_x, min_y - 15, player, distance );

        auto [ head, torso ] = player.get_part_screen_locations();
        draw_snapline( get_fov_center(), head, torso );

        overlay_mgr.render_list.add< render::rectangle >( ImVec2{ max_x, max_y }, ImVec2{ min_x, min_y }, color );
    }

    void features::name_esp( float x, float y, rbx::player player, std::int32_t distance )
    {
        if ( config_mgr.get< bool >( "name_esp" ) )
        {
            const auto size = config_mgr.get< bool >( "autoscale_names" ) ? clamp_distance( distance, 12 ) : 0.0f;

            overlay_mgr.render_list.add< render::text >(
                ImVec2{ x, y }, size, config_mgr.get< ImColor >( "name_esp_color" ), player.name(), true );
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