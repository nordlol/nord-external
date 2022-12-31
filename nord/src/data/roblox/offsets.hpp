#pragma once
#include <cstdint>
#include <memory>

namespace nord::rbx
{
    class offsets final
    {
       public:
        offsets() : print( 0 ), get_task_scheduler( 0 )
        {
        }

        std::uintptr_t print;               // standard_out::get()->print()
        std::uintptr_t get_task_scheduler;  // task_scheduler::get()

        bool initialize();

       private:
        bool init_get_task_scheduler();
    };

    // base class (constructs and saves address)
    class base
    {
       public:
        base( const std::uintptr_t address ) : address( address )
        {
        }

        std::uintptr_t get_address() const
        {
            return address;
        }

       private:
        std::uintptr_t address;
    };

    extern offsets offsets_mgr;
}  // namespace nord::rbx