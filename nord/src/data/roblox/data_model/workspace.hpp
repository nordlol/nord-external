#pragma once

#include "camera.hpp"
#include "instance.hpp"

namespace nord::rbx
{
    class workspace : public instance
    {
        using instance::instance;

       public:
        camera current_camera() const;

       private:
    };
}  // namespace nord::rbx