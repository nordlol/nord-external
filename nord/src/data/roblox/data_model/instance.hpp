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
        std::string name() const;

       private:
    };

    class instance : public base
    {
        using base::base;

       public:
        [[nodiscard]] virtual std::vector< instance > get_children() const;
        instance get_child_by_name( std::string_view name );
        instance get_child_by_class_name( std::string_view name );

        template< typename T >
        [[nodiscard]] std::vector< T > get_children_as()
        {
            std::vector< T > children;

            for ( const auto& child : get_children() )
                children.emplace_back( child.get_address() );

            return children;
        }

        template< typename T >
        T get_child_by_name( std::string_view name )
        {
            return get_child_by_name( name ).get_address();
        }

        template< typename T >
        T get_child_by_class_name( std::string_view name )
        {
            return get_child_by_class_name( name ).get_address();
        }

        bool operator==( const instance& other )
        {
            return this->get_address() == other.get_address();
        }

        bool operator!()
        {
            return !this->get_address();
        }

        std::string name() const;
        class_descriptor descriptor() const;
        instance parent() const;

       private:
    };
}  // namespace nord::rbx