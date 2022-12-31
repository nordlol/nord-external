#pragma once

#include <string>
#include <vector>

#include "../offsets.hpp"

namespace nord::rbx
{
    class class_descriptor : public base
    {
        using base::base;

       public:
        std::string name();

       private:
    };

    class instance : public base
    {
        using base::base;

       public:
        [[nodiscard]] std::vector< instance > get_children();
        instance get_child_by_name( std::string_view name );
        instance get_child_by_class_name( std::string_view name );

        template< typename T >
        [[nodiscard]] std::vector< T > get_children_as()
        {
            std::vector< T > children;

            for ( const auto& child : get_children() )
                children.emplace_back( child );

            return children;
        }

        template< typename T >
        T get_child_by_name( std::string_view name )
        {
            return get_child_by_name( name );
        }

        template< typename T >
        T get_child_by_class_name( std::string_view name )
        {
            return get_child_by_class_name( name );
        }

        std::string name();
        class_descriptor descriptor();

       private:
    };
}  // namespace nord::rbx