#pragma once

#include "../engine/engine.hpp"
#include "../world/primitive.hpp"
#include "instance.hpp"

namespace nord::rbx
{
    class part : public instance
    {
        using instance::instance;

       public:
        engine::cframe_t cfame() const;
        engine::vector3_t size() const;

        // required for rbx::contact_manager
        primitive primitive() const;
    };
}  // namespace nord::rbx