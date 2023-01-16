#pragma once

#include <tuple>
#include <unordered_set>

#include "instance.hpp"
#include "part.hpp"
#include "team.hpp"

namespace nord::rbx
{
    class player : public instance
    {
        using instance::instance;

       public:
        instance character() const;
        team team() const;
        bool is_teammate( player player ) const;

        std::tuple< part, part > get_parts();
        part get_root_part();

        bool is_body_part( const rbx::part& part );

       private:
        std::unordered_set< std::string_view > regular_body_parts = {
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