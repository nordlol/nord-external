#pragma once
#include <cstdint>
#include <memory>

#include "../process_hook.hpp"

namespace nord::rbx
{
    class offsets final
    {
       public:
        std::uintptr_t print = 0x10358F0;  // standard_out::get()->print()

        void initialize();

       private:

        constexpr std::uintptr_t rebase( std::uintptr_t address );
    };

    extern offsets offsets_mgr;
}  // namespace nord::rbx