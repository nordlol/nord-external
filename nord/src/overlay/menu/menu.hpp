#pragma once

#include "../../imgui/imgui.h"
#include "../../imgui/imgui_impl_glfw.h"
#include "../../imgui/imgui_impl_opengl3.h"

#include "../../utils/config/config.hpp"

#include <iostream>

namespace nord
{
    class menu
    {
       public:
        // called before the message loop in the overlay
        void initiate();

        // called every iteration
        void render();
    };

    extern menu menu_mgr;
}  // namespace nord