#pragma once

#include <iostream>

#include "../../data/roblox/data_model/instance.hpp"
#include "../../imgui/imgui.h"
#include "../../imgui/imgui_internal.h"
#include "../../imgui/imgui_impl_glfw.h"
#include "../../imgui/imgui_impl_opengl3.h"
#include "../../imgui/imgui_stdlib.h"
#include "../../utils/config/config.hpp"

#include <GLFW/glfw3.h>
#include <map>

namespace nord
{
    class menu
    {
       public:
        // called before the message loop in the overlay
        void initiate();

        // called every iteration
        void render();

       private:
        // ImGui checkbox wrapper function
        void imgui_checkbox( const char* const name, std::string_view feature );

        // renders explorer (used for debugging and fun)
        void render_explorer();
        void render_explorer_nodes( rbx::instance instance, bool require_children = false );
        void render_explorer_properties();

        rbx::instance selected;

        public:
        GLuint logo_texture = 0;
        ImFont* widget_normal = 0;
        ImFont* widget_small = 0;
        ImFont* icon_font = 0;
        ImFont* small_font = 0;

       private:
        bool create_window();
        void end();

        bool create_tab_bar();
        void end_tab_bar();

        bool create_tab(
            const char* label,
            const char* icon,
            const bool selected,
            int space,
            bool is_top_tab,
            const ImVec2& size_arg );

        bool create_holder( const char* holder );
        bool create_top_holder( const char* name, bool fill = true );
        bool create_bottom_section( const char* name );
        bool create_section( const char* name );
        bool checkmark( const char* label, const char* Desc, bool* v );
        bool slider( const char* label, const char* type, int* v, int v_min, int v_max );
        bool slider( const char* label, const char* type, float* v, int v_min, int v_max );
        bool dropdown( const char* label, int* current_item, const char* const items[], int items_count, int height_in_items );
        bool color_edit( const char* name, ImColor* col );
    };

    extern menu menu_mgr;
}  // namespace nord