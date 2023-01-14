#pragma once

#include <thread>
#include <array>

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
        void static_box_esp( rbx::player player );
        void dynamic_box_esp( rbx::player player );
        void name_esp( float x, float y, rbx::player player );
    };

    extern features feature_mgr;
}  // namespace nord
