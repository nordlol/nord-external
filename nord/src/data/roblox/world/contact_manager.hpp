#pragma once

#include <optional>

#include "../data_model/part.hpp"
#include "primitive.hpp"
#include "../engine/ray.hpp"

constexpr float MAX_SEARCH_DEPTH = 2048.0f;

namespace nord::rbx
{
    class contact_manager
    {
       public:
        static std::shared_ptr< contact_manager > get();

        std::optional< part > get_ray_hit(engine::ray unit_ray); // TODO: add ignore list

       private:
        // prevent creation of the class outside of the singleton class
        contact_manager()
        {
        }
    };
}  // namespace nord::rbx