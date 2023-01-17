#pragma once

#include <memory>
#include <mutex>
#include <vector>

#include "../../imgui/imgui.h"

namespace nord::render
{
    struct base
    {
        virtual void render( ImDrawList* dl ) = 0;
        virtual ~base() = default;
    };

    struct line : public base
    {
        line( ImVec2 point1, ImVec2 point2, ImColor col, float thickness = 1.0f )
            : point1{ point1 },
              point2{ point2 },
              col{ col },
              thickness{ thickness }
        {
        }

        ImVec2 point1, point2;
        ImColor col;
        float thickness;

        void render( ImDrawList* dl ) override;
    };

    struct rectangle : public base
    {
        rectangle( ImVec2 point1, ImVec2 point2, ImColor col ) : point1{ point1 }, point2{ point2 }, col{ col }
        {
        }

        ImVec2 point1, point2;
        ImColor col;

        void render( ImDrawList* dl ) override;
    };

    struct circle : public base
    {
        circle( ImVec2 position, float radius, ImColor col ) : position{ position }, radius{ radius }, col{ col }
        {
        }

        ImVec2 position;
        float radius;
        ImColor col;

        void render( ImDrawList* dl ) override;
    };

    struct triangle : public base
    {
        triangle( ImVec2 point1, ImVec2 point2, ImVec2 point3, ImColor col, float thickness = 1.0f, bool filled = false )
            : point1{ point1 },
              point2{ point2 },
              point3{ point3 },
              col{ col },
              thickness{ thickness },
              filled{ filled }

        {
        }

        ImVec2 point1;
        ImVec2 point2;
        ImVec2 point3;
        ImColor col;
        float thickness;
        bool filled;

        void render( ImDrawList* dl ) override;
    };

    struct text : public base
    {
        text( ImVec2 pos, float size, ImColor col, const std::string& text, bool center )
            : pos{ pos },
              size{ size },
              col{ col },
              val{ text },
              center{ center }
        {
        }

        float size;
        std::string val;
        ImColor col;
        ImVec2 pos;
        bool center;

        void render( ImDrawList* dl ) override;
    };

    class render_list
    {
       public:
        template< typename T, typename... Args >
        void add( const Args&... args )
        {
            temp.emplace_back( new T{ args... } );
        }

        void begin() noexcept;
        void end() noexcept;
        void clear() noexcept;
        void draw( ImDrawList* dl );
        void update() noexcept;

       private:
        std::vector< std::shared_ptr< base > > temp;

        std::vector< std::shared_ptr< base > > list;
        std::mutex mutex;
    };
}  // namespace nord::render
