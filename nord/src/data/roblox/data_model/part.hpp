#pragma once

#include "../engine/engine.hpp"
#include "instance.hpp"

namespace nord::rbx
{
    class part : public instance
    {
        using instance::instance;

       public:
        engine::cframe_t cfame() const;
        engine::vector3_t size() const;

       private:
        std::uintptr_t primitive() const;
    };
}  // namespace nord::rbx