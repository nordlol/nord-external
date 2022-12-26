#pragma once

#include <GLFW/glfw3.h>
#include <Windows.h>
#include <dwmapi.h>
#include <psapi.h>
#include <tlhelp32.h>

#include <iostream>

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

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

       private:
        // initializers / constructors
        bool setup_glflw();
        bool setup_imgui();

        // destructors
        void destroy_glflw();
        void destroy_imgui();

        const char *get_glsl_version();

        GLFWwindow *window = nullptr;
        const char *const title;
    };

    extern overlay overlay_mgr;
}  // namespace nord
