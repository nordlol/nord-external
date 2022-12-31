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

        if ( comp.w < 0.001f )
            return std::nullopt;

        comp.x /= comp.w;
        comp.y /= comp.w;

        return engine::vector2_t{ ( width / 2.0f ) + ( comp.x * width ) / 2.0f,
                                  ( height / 2.0f ) - ( comp.y * height ) / 2.0f };
    }
}  // namespace nord::rbx