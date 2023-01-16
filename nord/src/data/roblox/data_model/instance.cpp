#include "instance.hpp"

#include "../../process_hook.hpp"

namespace nord::rbx
{
    std::vector< instance > instance::get_children() const
    {
        const auto container = process_hook_mgr.mem.proc->read< std::uintptr_t >( get_address() + 0x28 );

        if ( container )
            return process_hook_mgr.mem.proc->read_vector< instance >( container );
        
        return std::vector< instance >();
    }

    instance instance::get_child_by_name( std::string_view name )
    {
        for ( const auto& child : get_children() )
        {
            if ( child.name() == name )
                return child;
        }

        return instance( 0 );
    }

    instance instance::get_child_by_class_name( std::string_view name )
    {
        for ( const auto& child : get_children() )
        {
            if ( child.descriptor().name() == name )
                return child;
        }

        return instance( 0 );
    }

    std::string instance::name() const
    {
        const auto ptr = process_hook_mgr.mem.proc->read< std::uintptr_t >( get_address() + 0x24 );

        if ( ptr )
            return process_hook_mgr.mem.proc->read_str( ptr );

        return std::string{};
    }

    class_descriptor instance::descriptor() const
    {
        return process_hook_mgr.mem.proc->read< std::uintptr_t >( get_address() + 0xC );
    }

    std::string class_descriptor::name() const
    {
        const auto ptr = process_hook_mgr.mem.proc->read< std::uintptr_t >( get_address() + 0x4 );
        return process_hook_mgr.mem.proc->read_str( ptr );
    }

    instance instance::parent() const
    {
        return process_hook_mgr.mem.proc->read< std::uintptr_t >( get_address() + 0x30 );
    }
}  // namespace nord::rbx
