#pragma once

#include <thread>

#include "../data/process_hook.hpp"
#include "../overlay/overlay.hpp"

namespace nord
{
    class features
    {
       public:
        void start();

       private:
        // processes visuals
        void run_visuals();
    };

    extern features feature_mgr;
}  // namespace nord
