#pragma once

#include <Windows.h>
#include <d3d9.h>

#include <iostream>

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"

#pragma comment( lib, "d3d9.lib" )

namespace nord
{
    class overlay
    {
       public:
        overlay( LPCWSTR name ) : name( name ), direct3d( nullptr ), device( nullptr ), params( { NULL } )
        {
        }

        /// <summary>
        /// Starts the main overlay window.
        /// </summary>
        void start();

        // directx interface variables
        IDirect3D9Ex* direct3d;
        IDirect3DDevice9Ex* device;
        D3DPRESENT_PARAMETERS params;

       private:
        // main update loop
        void update();

        void setup_window();
        bool setup_directx();
        void cleanup_directx();
        void extend_into_client();

        WNDCLASSEX window_class;
        HWND window;
        LPCWSTR name;
    };

    extern overlay overlay_mgr;
}  // namespace nord
