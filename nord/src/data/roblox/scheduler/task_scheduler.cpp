#include "task_scheduler.hpp"

namespace nord::rbx
{
    std::shared_ptr< task_scheduler > task_scheduler::get()
    {
        auto singleton = process_hook_mgr.mem.proc->create_sub< std::uintptr_t >( offsets_mgr.get_task_scheduler );
        return std::make_shared< task_scheduler >( singleton() );
    }

    // TODO: add read_vector to glad-ext
    std::vector< task_scheduler::job > task_scheduler::get_jobs()
    {
        const auto container = get_address() + 0x134;

        // get the scope of the vector
        auto start = process_hook_mgr.mem.proc->read< std::uintptr_t >( container );
        const auto end = process_hook_mgr.mem.proc->read< std::uintptr_t >( container + 0x4 );

        std::vector< task_scheduler::job > jobs;

        for ( ; start < end; start += 8 )
            jobs.emplace_back( process_hook_mgr.mem.proc->read< std::uintptr_t >( start ) );

        return jobs;
    }

    std::vector< task_scheduler::job > task_scheduler::get_jobs_by_name( std::string_view name )
    {
        std::vector< job > jobs;

        for ( auto& child : get_jobs() )
        {
            if ( child.name() == name )
                jobs.push_back( child );
        }

        return jobs;
    }

    std::string task_scheduler::job::name()
    {
        return process_hook_mgr.mem.proc->read_str( get_address() + 0x10 );
    }
}  // namespace nord::rbx