#pragma once

#include <iostream>

#include "../../imgui/imgui.h"
#include "../../imgui/imgui_impl_glfw.h"
#include "../../imgui/imgui_impl_opengl3.h"
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
    };

    extern menu menu_mgr;
}  // namespace nord