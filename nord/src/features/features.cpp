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

                    overlay_mgr.render_list.begin();
                    overlay_mgr.render_list.update();
                    overlay_mgr.render_list.end();
                }
            } );

        visuals.detach();
    }

    void features::run_visuals()
    {
        if ( config_mgr.get< bool >( "esp_enabled" ) )
        {
            const auto local_player = process_hook_mgr.players.local_player();

            for ( auto& player : process_hook_mgr.players.get_children_as< rbx::player >() )
            {
                if ( player.get_address() == local_player.get_address() )
                    continue;

                const auto character = player.character();

                if ( !character.get_address() )
                    continue;

                const auto [ head, root ] = player.get_parts();

                if ( !head.get_address() || !root.get_address() )
                    continue;

                const auto head_location = process_hook_mgr.visual_engine->world_to_screen(
                    root.cfame().translation + rbx::engine::vector3_t( 0, 0.9f, 0 ) );

                const auto torso_location = process_hook_mgr.visual_engine->world_to_screen(
                    root.cfame().translation - rbx::engine::vector3_t( 0, 0.4f, 0 ) );

                if ( head_location.has_value() && torso_location.has_value() )
                {
                    const auto [ head_x, head_y ] = head_location.value();
                    const auto [ torso_x, torso_y ] = torso_location.value();

                    const auto width = std::fabsf( head_y - torso_y );
                    const auto height = std::fabsf( head_y - torso_y ) * 2.2f;

                    overlay_mgr.render_list.add< render::rectangle >(
                        ImVec2{ torso_x - width, torso_y + height },
                        ImVec2{ torso_x + width, torso_y - height },
                        ImColor{ 255, 255, 255 } );
                }
            }
        }
    }

    features feature_mgr;
}  // namespace nord