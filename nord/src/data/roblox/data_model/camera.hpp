#pragma once

#include "instance.hpp"
#include "part.hpp"

#include "../engine/engine.hpp"

namespace nord::rbx
{
    class camera : public instance
    {
        using instance::instance;

       public:
        engine::cframe_t cframe() const;

        std::int32_t get_distance_from( part part ) const;
       private:
    };
}  // namespace nord::rbx