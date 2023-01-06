#include "instance.hpp"

namespace nord::rbx
{
    class humanoid : public instance
    {
        using instance::instance;

       public:
        float get_health() const;
        float get_max_health() const;

       private:
    };
}  // namespace nord::rbx