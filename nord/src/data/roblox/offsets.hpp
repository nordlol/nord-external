#pragma once
#include <cstdint>
#include <memory>

namespace nord::rbx
{
    class offsets final
    {
       public:
        offsets() : print( 0 ), get_task_scheduler( 0 ), get_health( 0 ), get_max_health( 0 )
        {
        }

        std::uintptr_t print;               // standard_out::get()->print()
        std::uintptr_t get_task_scheduler;  // task_scheduler::get()
        std::uintptr_t get_health;          // humanoid::get_health()
        std::uintptr_t get_max_health;      // humanoid::get_max_health()

        bool initialize();

       private:
        bool init_get_task_scheduler();
        bool init_print();
        bool init_get_health();
        bool init_get_max_health();
    };

    // base class (constructs and saves address)
    class base
    {
       public:
        base( const std::uintptr_t address ) : address( address )
        {
        }

        base() : address( 0 )
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