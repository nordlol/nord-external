#pragma once

#include "instance.hpp"

namespace nord::rbx
{
    enum class game_t : std::uint64_t
    {
        phantom_forces = 113491250,
        bad_buisness = 1168263273,
        rush_point = 2162282815,
        recoil = 1534453623,
        apocalypse_rising_2 = 358276974
    };

    class data_model : public instance
    {
       public:
        data_model( const std::uintptr_t address ) : instance( address + 4 )
        {
        }

        game_t game_id() const;

       private:
    };
}  // namespace nord::rbx
