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

        //ImGui::Checkbox( "ESP", config_mgr.get_ptr< bool >( "esp" ) );

        ImGui::End();
    }

    menu menu_mgr;

}  // namespace nord