#pragma once

#include "../engine/engine.hpp"
#include "../offsets.hpp"

namespace nord::rbx
{
    class primitive : public base
    {
        using base::base;

       public:
        engine::cframe_t cfame() const;
        engine::vector3_t size() const;
        float radius() const;
       private:
    };
}  // namespace nord::rbx