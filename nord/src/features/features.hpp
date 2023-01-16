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

        // visual features
        void static_box_esp( rbx::player player, ImColor color, std::int32_t distance );
        void dynamic_box_esp( rbx::player player, ImColor color, std::int32_t distance );
        void name_esp( float x, float y, rbx::player player, std::int32_t distance );

        float clamp_distance( std::int32_t distance, std::int32_t clamp );
    };

    extern features feature_mgr;
}  // namespace nord
