#pragma once

#include <optional>
#include <unordered_set>

#include "../data_model/part.hpp"
#include "../engine/ray.hpp"
#include "primitive.hpp"

namespace nord::rbx
{
    class contact_manager
    {
       public:
        static std::shared_ptr< contact_manager > get();

        constexpr static float MAX_SEARCH_DEPTH = 2048.0f;

        std::optional< part > get_ray_hit( engine::ray unit_ray, std::unordered_set< std::uintptr_t > ignore );

       private:
        // prevent creation of the class outside of the singleton class
        contact_manager()
        {
        }

        std::vector< part > get_parts( instance root, std::unordered_set< std::uintptr_t > ignore ) const;
    };
}  // namespace nord::rbx