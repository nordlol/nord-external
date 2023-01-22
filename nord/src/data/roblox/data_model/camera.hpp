#pragma once

#include "instance.hpp"
#include "part.hpp"

#include "../engine/engine.hpp"
#include "../engine/ray.hpp"

namespace nord::rbx
{
    class camera : public instance
    {
        using instance::instance;

       public:
        engine::cframe_t cframe() const;
        float fov() const;
        float image_plane_depth() const;

        std::int32_t get_distance_from( part part ) const;

        engine::ray screen_point_to_ray( float x, float y, float depth = 0 );
       private:
    };
}  // namespace nord::rbx