#include "features.hpp"

namespace nord
{
    void features::start()
    {
        std::thread visuals(
            [ & ]()
            {
                while ( true )
                {
                    overlay_mgr.render_list.begin();
                    run_visuals();
                    overlay_mgr.render_list.end();
                }
            } );

        visuals.detach();
    }

    void features::run_visuals()
    {
        overlay_mgr.render_list.add< render::rectangle >( ImVec2( 0, 0 ), ImVec2( 100, 100 ), ImColor{ 255, 255, 255 } );
    }

    features feature_mgr;
}  // namespace nord