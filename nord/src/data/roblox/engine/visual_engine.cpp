#include "visual_engine.hpp"

#include "../../process_hook.hpp"

namespace nord::rbx
{
    engine::matrix4_t visual_engine::view_matrix() const
    {
        return process_hook_mgr.mem.proc->read< engine::matrix4_t >( get_address() + 0x170 );
    }

    engine::vector2_t visual_engine::viewport() const
    {
        return process_hook_mgr.mem.proc->read< engine::vector2_t >( get_address() + 0x6FC );
    }

    std::optional< engine::vector2_t > visual_engine::world_to_screen( const engine::vector3_t& in ) const
    {
        const auto& [ width, height ] = viewport();

        engine::vector4_t comp = view_matrix() * in;

        if ( comp.w >= 0.001f )
        {
            comp.x /= comp.w;
            comp.y /= comp.w;

            const auto& ret = engine::vector2_t{ ( width / 2.0f ) + ( comp.x * width ) / 2.0f,
                                                 ( height / 2.0f ) - ( comp.y * height ) / 2.0f };

            if ( ret.x < 0 || ret.y < 0 || ret.x > comp.x || ret.y > comp.y )
                return ret;
        }

        return std::nullopt;
    }

    std::vector< engine::vector3_t > visual_engine::get_corners( engine::vector3_t position, engine::vector3_t size )
    {
        return {
            // Back face
            engine::vector3_t( position.x + size.x / 2, position.y + size.y / 2, position.z + size.z / 2 ),  // Top right
            engine::vector3_t( position.x - size.x / 2, position.y + size.y / 2, position.z + size.z / 2 ),  // Top left

            engine::vector3_t( position.x - size.x / 2, position.y - size.y / 2, position.z - size.z / 2 ),  // Bottom left
            engine::vector3_t( position.x + size.x / 2, position.y - size.y / 2, position.z - size.z / 2 ),  // Bottom right

            // Front face
            engine::vector3_t( position.x - size.x / 2, position.y + size.y / 2, position.z - size.z / 2 ),
            engine::vector3_t( position.x + size.x / 2, position.y + size.y / 2, position.z - size.z / 2 ),

            engine::vector3_t( position.x - size.x / 2, position.y - size.y / 2, position.z + size.z / 2 ),
            engine::vector3_t( position.x + size.x / 2, position.y - size.y / 2, position.z + size.z / 2 ),
        };
    }
}  // namespace nord::rbx