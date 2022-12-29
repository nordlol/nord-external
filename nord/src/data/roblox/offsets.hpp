#pragma once
#include <cstdint>
#include <memory>

namespace nord::rbx
{
    // namespace standard_out
    //{
    //    std::uintptr_t print = 0x7258E0;  // standard_out::get()->print()

    //}  // namespace standard_out

    class offsets final
    {
       public:
        static std::shared_ptr< offsets > get();


        std::uintptr_t print = 0x7258E0;  // standard_out::get()->print()

        void initialize();
       private:
        offsets()
        {
        }

        constexpr std::uintptr_t rebase( std::uintptr_t address );
    };
}  // namespace nord::rbx