#pragma once
#include <cstdint>

#include "../process_hook.hpp"

#define rebase( address ) ( process_hook_mgr.mem.module.base + address - 0x400000 );

namespace nord::rbx::offsets
{
    namespace standard_out
    {
        const auto print = rebase( 0x7258E0 );  // standard_out::get()->print()

    }  // namespace standard_out
}  // namespace nord::rbx::offsets