#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "../data_model/data_model.hpp"
#include "../engine/visual_engine.hpp"
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
            using base::base;

           public:
            // gets the job name
            std::string name();

            std::shared_ptr< data_model > game();
        };

        [[nodiscard]] std::vector< job > get_jobs();
        [[nodiscard]] std::vector< job > get_jobs_by_name( std::string_view name );
        bool set_frame_delay( std::int32_t delay = -1 );

        // addons
        std::shared_ptr< data_model > get_data_model();
        std::shared_ptr< visual_engine > get_visual_engine();

       private:
        std::uintptr_t frame_delay_offset = 0;
    };
}  // namespace nord::rbx