#include "overlay.hpp"

#include <thread>

#include "../data/process_hook.hpp"

namespace nord
{
    // error callback function for prettyness ;)
    static void glfw_error_callback( int error, const char* const description )
    {
        std::cout << "Error (" << error << "): " << description << std::endl;
    }

    void glfw_key_callback( GLFWwindow* window, int key, int scancode, int action, int mods )
    {
        if ( key == GLFW_KEY_INSERT && action == GLFW_PRESS )
            overlay_mgr.show_ui = !overlay_mgr.show_ui;

        glfwSetWindowAttrib( window, GLFW_MOUSE_PASSTHROUGH, !overlay_mgr.show_ui );
    }

    bool overlay::setup_glflw()
    {
        glfwSetErrorCallback( glfw_error_callback );

        if ( !glfwInit() )
            return false;

        glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );
        glfwWindowHint( GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE );

        // create our window
        if ( !( window = glfwCreateWindow( 800, 800, title, nullptr, nullptr ) ) )
        {
            glfwTerminate();
            return false;
        }

        glfwSetKeyCallback( window, glfw_key_callback );

        glfwSetWindowAttrib( window, GLFW_DECORATED, GLFW_FALSE );
        glfwSetWindowAttrib( window, GLFW_FLOATING, GLFW_TRUE );

        // make the window's context current
        glfwMakeContextCurrent( window );
        glfwShowWindow( window );
        glfwSwapInterval( 0 );
        return true;
    }

    bool overlay::setup_imgui()
    {
        // setup imgui context
        IMGUI_CHECKVERSION();
        if ( !ImGui::CreateContext() )
            return false;

        // setup imgui style
        ImGui::StyleColorsDark();

        // setup platform/renderer backends
        ImGui_ImplGlfw_InitForOpenGL( window, true );
        ImGui_ImplOpenGL3_Init( get_glsl_version() );
    }

    bool overlay::start()
    {
        // setup glflw window
        if ( !setup_glflw() )
            return false;

        // setup imgui instance
        if ( !setup_imgui() )
        {
            destroy_glflw();
            return false;
        }

        ImVec4 clear_color = ImVec4( 0.45f, 0.55f, 0.60f, 1.00f );

        // loop until the user closes roblox
        while ( process_hook_mgr.window != nullptr && !glfwWindowShouldClose( window ) )
        {
            glfwPollEvents();

            // Update window position and size
            glfwSetWindowPos( window, process_hook_mgr.screen.x, process_hook_mgr.screen.y );
            glfwSetWindowSize( window, process_hook_mgr.screen.width, process_hook_mgr.screen.height );

            render();

            glfwSwapBuffers( window );
        }

        // cleanup
        destroy_imgui();
        destroy_glflw();
        return true;
    }

    void overlay::render()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if ( show_ui )
            ImGui::ShowDemoWindow();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize( window, &display_w, &display_h );
        glViewport( 0, 0, display_w, display_h );
        glClearColor( 0.0f, 0.0f, 0.0f, show_ui ? 0.2f : 0.0f );
        glClear( GL_COLOR_BUFFER_BIT );
        ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );
    }

    void overlay::destroy_glflw()
    {
        glfwDestroyWindow( window );
        glfwTerminate();
    }

    void overlay::destroy_imgui()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    const char* overlay::get_glsl_version()
    {
        // decide GL+GLSL versions
#if defined( IMGUI_IMPL_OPENGL_ES2 )
        // GL ES 2.0 + GLSL 100
        const char* glsl_version = "#version 100";
        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 2 );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0 );
        glfwWindowHint( GLFW_CLIENT_API, GLFW_OPENGL_ES_API );
#elif defined( __APPLE__ )
        // GL 3.2 + GLSL 150
        const char* glsl_version = "#version 150";
        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 2 );
        glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );  // 3.2+ only
        glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );            // Required on Mac
#else
        // GL 3.0 + GLSL 130
        const char* glsl_version = "#version 130";
        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0 );
        // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
        // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif
        return glsl_version;
    }

    overlay overlay_mgr{ "nord" };
}  // namespace nord