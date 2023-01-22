#include "camera.hpp"

#include <iostream>
#include <algorithm>

#include "../../process_hook.hpp"

namespace nord::rbx
{
    engine::cframe_t camera::cframe() const
    {
        return process_hook_mgr.mem.proc->read< engine::cframe_t >( get_address() + 0x78 );
    }

    float camera::fov() const
    {
        return process_hook_mgr.mem.proc->read< float >( get_address() + 456 ) * 180 / pi;
    }

    float camera::image_plane_depth() const
    {
        return static_cast< float >( 1.0f / ( 2.0f * std::tan( fov() / 2.0f ) ) );
    }

    std::int32_t camera::get_distance_from( part part ) const
    {
        return ( cframe().translation - part.cfame().translation ).magnitude();
    }

    engine::ray camera::screen_point_to_ray( float x, float y, float depth )
    {
        // center coordinates
        const auto [ cx, cy ] = process_hook_mgr.visual_engine->viewport() / 2.0f;

        const auto origin = cframe().translation;
        const auto point = engine::vector3_t{ ( x / cx ) - 1.0f, 1.0f - ( y / cy ), image_plane_depth() };

        const auto projection = process_hook_mgr.visual_engine->view_matrix();
        const auto projected_point = projection.inverse() * engine::vector4_t{ point, 1.0f };
        const auto projected_point_adjusted = projected_point.xyz() / projected_point.w;
        auto direction = projected_point_adjusted - origin;

        direction = direction.direction();

        const auto distance = direction.dot( cframe().look_vector() ); 
        const float theta = std::acos( min( 1.0f, distance ) );
        const float depth_to_near_clip_plane = image_plane_depth() / sin( ( pi / 2 ) - theta );

        return { origin + ( direction * depth_to_near_clip_plane ) + ( direction * depth ), direction };
    }
}  // namespace nord::rbx