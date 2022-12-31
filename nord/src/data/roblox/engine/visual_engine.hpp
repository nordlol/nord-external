#pragma once

#include <optional>

#include "../offsets.hpp"
#include "engine.hpp"

namespace nord::rbx
{
    class visual_engine : public base
    {
        using base::base;

       public:
        engine::matrix4_t view_matrix() const;
        engine::vector2_t viewport() const;

        std::optional< engine::vector2_t > world_to_screen( const engine::vector3_t& in ) const;

       private:
    };
}  // namespace nord::rbx
