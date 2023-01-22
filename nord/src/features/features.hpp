#pragma once

#include <array>
#include <thread>

#include "../data/process_hook.hpp"
#include "../overlay/overlay.hpp"

namespace nord
{
    class features
    {
       public:
        void start();

       private:
        // processes visuals
        void run_visuals();

        // processes other features
        void run_other();

        // visual features
        void static_box_esp( rbx::player player, ImColor color, std::int32_t distance );
        void dynamic_box_esp( rbx::player player, ImColor color, std::int32_t distance );
        void name_esp( float x, float y, rbx::player player, std::int32_t distance );
        void draw_fov_circle();
        void draw_snapline( rbx::engine::vector2_t pos, const ImVec2& head, const ImVec2& torso );

        float clamp_distance( std::int32_t distance, std::int32_t clamp );
        ImColor get_player_color( rbx::player player, bool teammate );
        rbx::engine::vector2_t get_fov_center();
    };

    extern features feature_mgr;
}  // namespace nord
