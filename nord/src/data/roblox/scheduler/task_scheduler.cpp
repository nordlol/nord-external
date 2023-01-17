#include "task_scheduler.hpp"

#include <memory>

#include "../../process_hook.hpp"

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

        return process_hook_mgr.mem.proc->read_vector< task_scheduler::job >( container );
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

    bool task_scheduler::set_frame_delay( std::int32_t delay )
    {
        if ( !frame_delay_offset )
        {
            // locate frame day offset
            for ( size_t offset = 0; offset < 0x500; offset += 0x4 )
            {
                if ( ( 1.0 / process_hook_mgr.mem.proc->read< double >( get_address() + offset ) ) == 60.0 )
                    frame_delay_offset = offset;
            }

            // couldn't locate
            if ( !frame_delay_offset )
                return false;
        }

        process_hook_mgr.mem.proc->write< double >(
            get_address() + frame_delay_offset, delay > 0 ? 1.0 / delay : 1.0 / 10000.0 );
        return true;
    }

    std::shared_ptr< data_model > task_scheduler::get_data_model()
    {
        std::vector< job > jobs = get_jobs_by_name( "WaitingHybridScriptsJob" );

        if ( !jobs.size() )
        {
            log_mgr.log_error( "task_scheduler", "Unable to locate job: \"WaitingHybridScriptsJob\"" );
            return nullptr;
        }

        return jobs.back().game();
    }

    std::shared_ptr< visual_engine > task_scheduler::get_visual_engine()
    {
        std::vector< job > jobs = get_jobs_by_name( "Render" );

        if ( !jobs.size() )
        {
            log_mgr.log_error( "task_scheduler", "Unable to locate job: \"Render\"" );
            return nullptr;
        }

        const auto render_view = process_hook_mgr.mem.proc->read< std::uintptr_t >( jobs.back().get_address() + 0x148 );
        const auto engine = process_hook_mgr.mem.proc->read< std::uintptr_t >( render_view + 0x8 );

        if (engine)
            return std::make_shared< visual_engine >( engine );
        
        return nullptr;
    }

    std::string task_scheduler::job::name()
    {
        return process_hook_mgr.mem.proc->read_str( get_address() + 0x10 );
    }

    std::shared_ptr< data_model > task_scheduler::job::game()
    {
        return std::make_shared< data_model >( process_hook_mgr.mem.proc->read< std::uintptr_t >( get_address() + 0x28 ) );
    }
}  // namespace nord::rbx