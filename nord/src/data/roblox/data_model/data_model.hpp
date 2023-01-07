#pragma once

#include "instance.hpp"

namespace nord::rbx
{
    class data_model : public instance
    {
       public:
        data_model( const std::uintptr_t address ) : instance( address + 4 )
        {
        }

        std::int64_t game_id() const;

       private:
    };
}  // namespace nord::rbx
