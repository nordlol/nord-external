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

        std::array< std::string_view, 20 > body_parts = {
            "Head",

            // R16 character body parts
            "UpperTorso",
            "LowerTorso",
            "LeftUpperArm",
            "RightUpperArm",
            "RightLowerArm",
            "LeftLowerArm",
            "LeftHand",
            "RightHand",
            "LeftUpperLeg",
            "RightUpperLeg",
            "LeftLowerLeg",
            "RightLowerLeg",
            "LeftFoot",
            "RightFoot",

            // R6 character body parts
            "Torso",
            "Right Arm",
            "Left Arm",
            "Left Leg",
            "Right Leg",
        };
    };

    extern features feature_mgr;
}  // namespace nord
