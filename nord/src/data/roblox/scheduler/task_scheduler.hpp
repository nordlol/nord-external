#pragma once

#include <vector>

#include "../offsets.hpp"

namespace nord::rbx
{
    class task_scheduler : public base
    {
        using base::base;

       public:
        static std::shared_ptr< task_scheduler > get();

        class job : public base
        {
           public:
            // gets the job name
            std::string name();
        };

        [[nodiscard]] std::vector< job > get_jobs();
        [[nodiscard]] std::vector< job > get_jobs_by_name( std::string_view name );
        bool set_frame_delay( std::int32_t delay = -1 );

       private:
        std::uintptr_t frame_delay_offset = 0;
    };
}  // namespace nord::rbx