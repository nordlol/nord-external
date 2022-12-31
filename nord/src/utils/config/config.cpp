#include "config.hpp"

#include <sstream>

namespace nord
{
    std::string feature::serialize()
    {
        if ( std::holds_alternative< settings_types::virtual_key >( value ) )
        {
            const auto& val = std::get< settings_types::virtual_key >( value );
            std::uint32_t scan_code = MapVirtualKey( val, MAPVK_VK_TO_VSC );

            // do some bitshifting
            if ( settings_types::is_key_extended( val ) )
                scan_code |= KF_EXTENDED;

            // do some weird stuff to converty virtual key to string
            WCHAR name[ VKEY_MAX ];
            if ( GetKeyNameText( scan_code << 16, name, VKEY_MAX ) )
            {
                std::wstring ws{ name };
                return { ws.begin(), ws.end() };
            }
        }
        else if ( std::holds_alternative< bool >( value ) )
            return std::string{ std::get< bool >( value ) ? "true" : "false" };
        else if ( std::holds_alternative< std::int32_t >( value ) )
            return std::to_string( std::get< std::int32_t >( value ) );
    }

    std::string configuration::serialize()
    {
        std::stringstream ss;

        for ( auto& entry : feature_map )
            ss << entry.first << ": " << entry.second.serialize() << '\n';

        return ss.str();
    }

    configuration config_mgr{};
}  // namespace nord