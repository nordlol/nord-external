#pragma once

#include <iostream>

#include "../../data/roblox/data_model/instance.hpp"
#include "../../imgui/imgui.h"
#include "../../imgui/imgui_impl_glfw.h"
#include "../../imgui/imgui_impl_opengl3.h"
#include "../../imgui/imgui_stdlib.h"
#include "../../utils/config/config.hpp"

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
    };

    extern menu menu_mgr;
}  // namespace nord