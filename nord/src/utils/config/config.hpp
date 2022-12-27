#pragma once

#include <windows.h>

#include <iostream>
#include <string_view>
#include <unordered_map>
#include <variant>

#include "../../imgui/imgui.h"

constexpr std::int32_t VKEY_MAX = 1024;

namespace nord
{
    namespace settings_types
    {
        typedef std::uint8_t virtual_key;

        constexpr bool is_key_extended( virtual_key key )
        {
            return key == VK_UP || key == VK_LEFT || key == VK_RIGHT || key == VK_DOWN || key == VK_RCONTROL ||
                   key == VK_RMENU || key == VK_LWIN || key == VK_RWIN || key == VK_APPS || key == VK_PRIOR ||
                   key == VK_NEXT || key == VK_END || key == VK_HOME || key == VK_INSERT || key == VK_DELETE ||
                   key == VK_DIVIDE || key == VK_NUMLOCK;
        }
    }  // namespace settings_types

    class feature
    {
       public:
        std::string serialize();

        template< typename T >
        feature( T value ) : value{ value }
        {
        }

        template< typename T >
        T& get_value()
        {
            return std::get< T >( value );
        }

       private:
        std::variant< bool, float, int, ImColor, settings_types::virtual_key > value;
    };

    class configuration
    {
       public:
        template< typename T >
        T& get( std::string_view feature )
        {
            return feature_map.at( feature ).get_value< T >();
        }

        // serializes feature_map to readable format
        std::string serialize();

       private:
        std::unordered_map< std::string_view, feature > feature_map = {
            // ui settings
            { "ui_toggle_key", feature{ VK_INSERT } },

            // esp
            { "esp_enabled", feature{ true } },
        };
    };

    extern configuration config_mgr;
}  // namespace nord
