#pragma once

#include <mutex>
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

        void start();
        void set_ignore_descendants( std::unordered_set< std::uintptr_t > ignore );

        std::optional< part > get_ray_hit( engine::ray unit_ray );

       private:
        // prevent creation of the class outside of the singleton class
        contact_manager()
        {
        }

        std::vector< part > get_parts( instance root );

        std::vector< part > parts;
        std::unordered_set< std::uintptr_t > ignore;
        std::mutex get_parts_lock, set_ignore_lock;
    };
}  // namespace nord::rbx