#include "humanoid.hpp"

#include "../../process_hook.hpp"

namespace nord::rbx
{
    double humanoid::get_health() const
    {
        auto sub = process_hook_mgr.mem.proc->create_sub< double >( offsets_mgr.get_health );
        printf( "%x\n", get_address() );
        return sub( ( int )get_address() );
    }

    float humanoid::get_max_health() const
    {
        return 0.0f;
    }

}  // namespace nord::rbx
