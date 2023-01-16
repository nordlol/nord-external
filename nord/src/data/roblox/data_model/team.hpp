#pragma once

#include "../util/brick_color.hpp"
#include "instance.hpp"

namespace nord::rbx
{
    class team : public instance
    {
        using instance::instance;

       public:
        brick_color team_color();

       private:
    };
}  // namespace nord::rbx