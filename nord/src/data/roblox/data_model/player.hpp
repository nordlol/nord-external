#pragma once

#include "instance.hpp"

namespace nord::rbx
{
    class player : public instance
    {
        using instance::instance;

       public:
        rbx::instance character() const;
       private:
    };
}  // namespace nord::rbx