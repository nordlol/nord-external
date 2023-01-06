#include "menu.hpp"

namespace nord
{
    void menu::initiate()
    {
        // define any styles in here
    }

    void menu::render()
    {
        ImGui::Begin( "window", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse );

        imgui_checkbox( "esp toggle", "esp" );

        ImGui::End();
    }

    void menu::imgui_checkbox( const char* const name, std::string_view feature )
    {
        bool value = config_mgr.get< bool >( feature );

        ImGui::Checkbox( name, &value );

        config_mgr.set< bool >( feature, value );
    }

    menu menu_mgr;

}  // namespace nord