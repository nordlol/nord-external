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
        const auto properties_height = 400;

        const auto viewport = ImGui::GetMainViewport()->Size;

        ImGui::SetNextWindowSize( ImVec2( explorer_width, viewport.y - properties_height ) );
        ImGui::SetNextWindowPos( ImVec2( viewport.x - explorer_width, 0 ) );

        ImGui::Begin(
            "explorer", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse );

        std::string s;
        ImGui::InputText( "Search", &s );

        ImGui::NewLine();

        render_explorer_nodes( *process_hook_mgr.data_model, true );

        ImGui::End();

        // draw the properties tab (bottom right)
        ImGui::SetNextWindowSize( ImVec2( explorer_width, properties_height ) );
        ImGui::SetNextWindowPos( ImVec2( viewport.x - explorer_width, viewport.y - properties_height ) );
        ImGui::Begin(
            "properties", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse );

        render_explorer_properties();

        ImGui::End();
    }

    void menu::render_explorer_nodes( rbx::instance instance, bool require_children )
    {
        for ( const auto& child : instance.get_children() )
        {
            const auto size = child.get_children().size();

            ImGui::PushID( "##unique_id" );

            if ( !size )
            {
                if ( require_children )
                    continue;

                ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
                ImGui::TreeNodeEx( child.name().c_str(), flags );

                ImGui::PopID();
            }

            else if ( ImGui::TreeNode( child.name().c_str() ) )
            {
                ImGui::PopID();

                render_explorer_nodes( child );
                ImGui::TreePop();
            }

            if ( ImGui::IsItemClicked() )
                selected = child;
        }
    }

    void menu::render_explorer_properties()
    {
        if ( selected.get_address() )
        {
            ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 2, 2 ) );
            if ( ImGui::BeginTable( "split", 2, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable ) )
            {
                // name
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex( 0 );
                ImGui::AlignTextToFramePadding();

                ImGui::Text( "Name" );

                ImGui::TableSetColumnIndex( 1 );
                ImGui::SetNextItemWidth( -FLT_MIN );

                ImGui::Text( selected.name().c_str() );
                ImGui::NextColumn();

                // classname
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex( 0 );
                ImGui::AlignTextToFramePadding();

                ImGui::Text( "ClassName" );

                ImGui::TableSetColumnIndex( 1 );
                ImGui::SetNextItemWidth( -FLT_MIN );

                ImGui::Text( selected.descriptor().name().c_str() );
                ImGui::NextColumn();

                // parent
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex( 0 );
                ImGui::AlignTextToFramePadding();

                ImGui::Text( "Parent" );

                ImGui::TableSetColumnIndex( 1 );
                ImGui::SetNextItemWidth( -FLT_MIN );

                ImGui::Text( selected.parent().name().c_str() );
                ImGui::NextColumn();
                ImGui::EndTable();
            }
            ImGui::PopStyleVar();
        }
    }

    menu menu_mgr;

}  // namespace nord