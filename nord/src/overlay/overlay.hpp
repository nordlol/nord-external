#pragma once

#include <GLFW/glfw3.h>
#include <Windows.h>
#include <dwmapi.h>
#include <psapi.h>
#include <tlhelp32.h>

#include <iostream>

#include "menu/menu.hpp"

#pragma comment( lib, "dwmapi.lib" )

namespace nord
{
    class overlay
    {
       public:
        overlay( const char *const title ) : title( title )
        {
        }

        // starts the overlay window. returns false if something goes wrong with glfw
        bool start();

        bool show_ui = false;
        bool is_focused = true;

       private:
        // initializers / constructors
        bool setup_glflw();
        void setup_imgui();

        void render();

        // destructors
        void destroy_glflw();
        void destroy_imgui();

        const char *get_glsl_version();

        GLFWwindow *window = nullptr;
        const char *const title;
    };

    extern overlay overlay_mgr;
}  // namespace nord
