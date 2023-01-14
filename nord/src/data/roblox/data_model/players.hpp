#pragma once

#include "instance.hpp"
#include "player.hpp"

namespace nord::rbx
{
    class players : public instance
    {
        using instance::instance;

       public:
        player local_player() const;

        // overriding
        [[nodiscard]] std::vector< instance > get_children() const;
       private:
    };
}  // namespace nord::rbx