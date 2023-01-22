#pragma once

#include "engine.hpp"

namespace nord::rbx::engine
{
    class ray
    {
       public:
        ray( const vector3_t& origin, const vector3_t& direction ) : origin( origin ), direction( direction )
        {
        }

        inline const vector3_t& get_origin() const
        {
            return origin;
        }

        inline const vector3_t& get_direction() const
        {
            return direction;
        }

       private:
        vector3_t origin;
        vector3_t direction;
    };
}  // namespace nord::rbx