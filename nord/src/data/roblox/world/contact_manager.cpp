#include "contact_manager.hpp"

namespace nord::rbx
{
    std::shared_ptr< contact_manager > contact_manager::get()
    {
        static std::shared_ptr< contact_manager > instance( new contact_manager() );
        return instance;
    }
    std::optional< part > contact_manager::get_ray_hit( engine::ray unit_ray )
    {
        return std::optional< part >();
    }
}