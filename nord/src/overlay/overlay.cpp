#include "overlay.hpp"

#include <dwmapi.h>

#include <thread>

#include "../data/process_hook.hpp"

LRESULT CALLBACK WinProc( HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam );

namespace nord
{
    void overlay::start()
    {
        while ( !process_hook_mgr.window_active )
            ;

        setup_window();

        if ( !setup_directx() )
        {
            MessageBox(
                nullptr,
                L"Unable to set up the DirectX device. Try restarting the application or contact an administrator.",
                L"Error",
                MB_ICONERROR | MB_OK );
            ExitProcess( EXIT_FAILURE );
        }

        std::thread t(
            [ & ]
            {
                while ( true )
                    update();
            } );

        t.detach();
    }

    bool overlay::setup_directx()
    {
        if ( FAILED( Direct3DCreate9Ex( D3D_SDK_VERSION, &direct3d ) ) )
            return false;

        D3DPRESENT_PARAMETERS Params = { 0 };
        Params.Windowed = TRUE;
        Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
        Params.hDeviceWindow = window;
        Params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
        Params.BackBufferFormat = D3DFMT_A8R8G8B8;
        Params.BackBufferWidth = process_hook_mgr.screen.width;
        Params.BackBufferHeight = process_hook_mgr.screen.height;
        Params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
        Params.EnableAutoDepthStencil = TRUE;
        Params.AutoDepthStencilFormat = D3DFMT_D16;
        Params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
        Params.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

        if ( FAILED( direct3d->CreateDeviceEx(
                 D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window, D3DCREATE_HARDWARE_VERTEXPROCESSING, &Params, 0, &device ) ) )
        {
            direct3d->Release();
            return false;
        }

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        ( void )io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImGui_ImplWin32_Init( window );
        ImGui_ImplDX9_Init( device );
        direct3d->Release();
        return true;
    }

    void overlay::cleanup_directx()
    {
        ImGui_ImplDX9_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();

        if ( device )
        {
            device->EndScene();
            device->Release();
        }

        if ( direct3d )
            direct3d->Release();

        DestroyWindow( window );
        UnregisterClass( window_class.lpszClassName, window_class.hInstance );
    }

    void overlay::extend_into_client()
    {
        MARGINS margin = { -1 };
        DwmExtendFrameIntoClientArea( window, &margin );
    }

    void overlay::update()
    {
        if ( !process_hook_mgr.window_active )
            return;

        MSG message{};

        while ( message.message != WM_QUIT )
        {
            if ( PeekMessage( &message, window, 0, 0, PM_REMOVE ) )
            {
                TranslateMessage( &message );
                DispatchMessage( &message );
            }

            // Update screen location and size
            if ( !IsIconic( window ) )
                MoveWindow(
                    window,
                    process_hook_mgr.screen.x,
                    process_hook_mgr.screen.y,
                    process_hook_mgr.screen.width,
                    process_hook_mgr.screen.height,
                    true );
        }

        cleanup_directx();
    }

    void overlay::setup_window()
    {
        // create window class instance
        window_class = { sizeof( WNDCLASSEX ),
                         0,
                         WinProc,
                         0,
                         0,
                         nullptr,
                         LoadIcon( nullptr, IDI_APPLICATION ),
                         LoadCursor( nullptr, IDC_ARROW ),
                         nullptr,
                         nullptr,
                         name,
                         LoadIcon( nullptr, IDI_APPLICATION ) };

        RegisterClassEx( &window_class );

        // create the new window
        window = CreateWindowEx(
            WS_EX_TOPMOST | WS_EX_LAYERED,
            name,
            name,
            WS_POPUP,
            process_hook_mgr.screen.x,
            process_hook_mgr.screen.y,
            process_hook_mgr.screen.width,
            process_hook_mgr.screen.height,
            nullptr,
            nullptr,
            0,
            nullptr );
        extend_into_client();

        // make window transparent
        SetWindowLong( window, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW );

        // show the window and upadate it's state
        ShowWindow( window, SW_SHOW );
        UpdateWindow( window );
    }

    overlay overlay_mgr{ L"nord" };
}  // namespace nord

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

LRESULT CALLBACK WinProc( HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam )
{
    if ( ImGui_ImplWin32_WndProcHandler( hWnd, Message, wParam, lParam ) )
        return true;

    switch ( Message )
    {
        case WM_DESTROY:
            if ( nord::overlay_mgr.device )
            {
                nord::overlay_mgr.device->EndScene();
                nord::overlay_mgr.device->Release();
            }

            if ( nord::overlay_mgr.direct3d )
                nord::overlay_mgr.direct3d->Release();

            PostQuitMessage( 0 );
            exit( 4 );
            break;
        case WM_SIZE:
            if ( nord::overlay_mgr.device && wParam != SIZE_MINIMIZED )
            {
                ImGui_ImplDX9_InvalidateDeviceObjects();
                nord::overlay_mgr.params.BackBufferWidth = LOWORD( lParam );
                nord::overlay_mgr.params.BackBufferHeight = HIWORD( lParam );
                HRESULT hr = nord::overlay_mgr.device->Reset( &nord::overlay_mgr.params );
                if ( hr == D3DERR_INVALIDCALL )
                    IM_ASSERT( 0 );
                ImGui_ImplDX9_CreateDeviceObjects();
            }
            break;
        default: return DefWindowProc( hWnd, Message, wParam, lParam ); break;
    }
    return 0;
}