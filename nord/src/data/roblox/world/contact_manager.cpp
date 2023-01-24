#include "contact_manager.hpp"

#include <execution>
#include <numbers>

#include "../../process_hook.hpp"

namespace nord::rbx
{
    std::shared_ptr< contact_manager > contact_manager::get()
    {
        static std::shared_ptr< contact_manager > instance( new contact_manager() );
        return instance;
    }

    std::optional< part > contact_manager::get_ray_hit( engine::ray unit_ray, std::unordered_set< std::uintptr_t > ignore )
    {
        part best_part;
        float best_perp_offset = FLT_MAX;  // I hate windows programming
        const std::vector< part > parts = get_parts( process_hook_mgr.workspace, ignore );

        for ( const auto& part : parts )
        {
            const primitive p = part.primitive();

            const auto world_position = p.cfame().translation;
            const float radius = p.radius();

            const auto ray_to_model = world_position - unit_ray.get_origin();
            const float ray_to_perp_point = unit_ray.get_direction().dot( ray_to_model );

            const auto perp_point_on_ray = unit_ray.get_origin() + ( unit_ray.get_direction() * ray_to_perp_point );
            const auto perp_point_to_center = world_position - perp_point_on_ray;
            const float perp_offset = perp_point_to_center.magnitude();

            if ( perp_offset <= radius && best_perp_offset > perp_offset )
            {
                best_part = part;
                best_perp_offset = perp_offset;
            }
        }

        if ( !best_part.get_address() )
            return std::nullopt;

        return best_part;
    }

    std::vector< part > contact_manager::get_parts( instance root, std::unordered_set< std::uintptr_t > ignore ) const
    {
        std::vector< part > parts;

        const auto children = root.get_children();

        for ( const auto& child : children )
        {
            // skip anything in the ignore set
            if ( ignore.find( child.get_address() ) != ignore.end() )
                continue;

            const auto internal_parts = get_parts( child, ignore );
            parts.insert( parts.end(), internal_parts.begin(), internal_parts.end() );

            if ( child.descriptor().name() != "Part" )
                continue;

            parts.emplace_back( child.get_address() );
        }

        return parts;
    }
}  // namespace nord::rbx