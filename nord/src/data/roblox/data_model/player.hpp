#pragma once

#include <tuple>

#include "instance.hpp"
#include "part.hpp"

namespace nord::rbx
{
    class player : public instance
    {
        using instance::instance;

       public:
        instance character() const;

        std::tuple<part, part> get_parts();

       private:
    };
}  // namespace nord::rbx