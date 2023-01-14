#pragma once

#include <tuple>
#include <array>

#include "instance.hpp"
#include "part.hpp"

namespace nord::rbx
{
    class player : public instance
    {
        using instance::instance;

       public:
        instance character() const;
        instance team() const;
        bool is_teammate(player player) const;

        std::tuple< part, part > get_parts();
        bool is_body_part( const rbx::part& part );

       private:
        std::array< std::string_view, 20 > regular_body_parts = {
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
}  // namespace nord::rbx