#pragma once

#include <vector>

#include "../offsets.hpp"

namespace nord::rbx
{
    class task_scheduler : public base
    {
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

       private:
    };
}  // namespace nord::rbx