#include "menu.hpp"

#include "../../data/process_hook.hpp"

namespace nord
{
    void menu::initiate()
    {
        // define any styles in here
    }

    void menu::render()
    {
        if ( config_mgr.get< bool >( "explorer" ) )
            render_explorer();

        ImGui::Begin( "window", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse );

        imgui_checkbox( "esp toggle", "esp" );
        imgui_checkbox( "explorer toggle", "explorer" );

        ImGui::End();
    }

    void menu::imgui_checkbox( const char* const name, std::string_view feature )
    {
        bool value = config_mgr.get< bool >( feature );

        ImGui::Checkbox( name, &value );

        config_mgr.set< bool >( feature, value );
    }

    void menu::render_explorer()
    {
        const auto explorer_width = 400;
        const auto viewport = ImGui::GetMainViewport()->Size;

        ImGui::SetNextWindowSize( ImVec2( explorer_width, viewport.y ) );
        ImGui::SetNextWindowPos( ImVec2( viewport.x - explorer_width, 0 ) );

        ImGui::Begin( "explorer", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove );

        std::string s;
        ImGui::InputText( "Search", &s );

        ImGui::NewLine();

        render_explorer_nodes( *process_hook_mgr.data_model, true );

        ImGui::End();
    }

    void menu::render_explorer_nodes( rbx::instance instance, bool require_children )
    {
        for ( const auto& child : instance.get_children() )
        {
            const auto size = child.get_children().size();

            if ( !size )
            {
                if ( require_children )
                    continue;

                ImGui::Text( ( "   " + child.name() ).c_str() );
            }

            else if ( ImGui::TreeNode( child.name().c_str() ) )
            {
                render_explorer_nodes( child );
                ImGui::TreePop();
            }
        }
    }

    menu menu_mgr;

}  // namespace nord