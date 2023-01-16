#include "render_list.hpp"

namespace nord::render
{
    void rectangle::render( ImDrawList* dl )
    {
        dl->AddRect( point1, point2, col );
    }

    void text::render( ImDrawList* dl )
    {
        if ( center )
            dl->AddText(
                nullptr, size, ImVec2( pos.x - ImGui::CalcTextSize( val.c_str() ).x / 2.0f, pos.y ), col, val.c_str() );
        else
            dl->AddText( nullptr, size, ImVec2( pos.x, pos.y ), col, val.c_str() );
    }

    void circle::render( ImDrawList* dl )
    {
        dl->AddCircle( ImVec2( position.x, position.y), radius, col );
    }

    void render_list::begin() noexcept
    {
        mutex.lock();
    }

    void render_list::end() noexcept
    {
        mutex.unlock();
    }

    void render_list::clear() noexcept
    {
        list.clear();
    }

    void render_list::update() noexcept
    {
        mutex.lock();
        list = temp;
        temp.clear();
        mutex.unlock();
    }

    void render_list::draw( ImDrawList* dl )
    {
        for ( const auto& item : list )
            item->render( dl );
    }
}  // namespace nord::render