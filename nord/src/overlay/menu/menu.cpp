#include "menu.hpp"

#include "../../data/process_hook.hpp"
#include "fonts/font-awesome.h"

static inline ImVec2 operator+( const ImVec2& lhs, const ImVec2& rhs )
{
    return ImVec2( lhs.x + rhs.x, lhs.y + rhs.y );
}
static inline ImVec2 operator-( const ImVec2& lhs, const ImVec2& rhs )
{
    return ImVec2( lhs.x - rhs.x, lhs.y - rhs.y );
}

namespace menu_mgr_utils
{
    bool SliderScalar(
        const char* label,
        const char* type,
        ImGuiDataType data_type,
        void* p_data,
        const void* p_min,
        const void* p_max,
        const char* format,
        ImGuiSliderFlags flags )
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if ( window->SkipItems )
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID( label );
        const float w = ImGui::CalcItemWidth();
        const ImVec2 label_size = ImGui::CalcTextSize( label, NULL, true );

        const ImRect total_bb(
            window->DC.CursorPos,
            window->DC.CursorPos + ImVec2( window->Size.x - g.Style.WindowPadding.x * 2 + 27, label_size.y + 20 ) );
        const ImRect frame_bb(
            ImVec2( window->DC.CursorPos.x + 10, window->DC.CursorPos.y - 10 ),
            ImVec2( total_bb.Max.x - 20, total_bb.Max.y ) );

        // ImGui::RenderFrame(frame_bb.Min, frame_bb.Max, ImColor(255, 255, 255), true, style.FrameRounding);

        const bool temp_input_allowed = ( flags & ImGuiSliderFlags_NoInput ) == 0;
        ImGui::ItemSize( total_bb, style.FramePadding.y );
        if ( !ImGui::ItemAdd( total_bb, id, &frame_bb, temp_input_allowed ? ImGuiItemFlags_Inputable : 0 ) )
            return false;

        // Default format string when passing NULL
        if ( format == NULL )
            format = ImGui::DataTypeGetInfo( data_type )->PrintFmt;

        // Tabbing or CTRL-clicking on Slider turns it into an input box
        const bool hovered = ImGui::ItemHoverable( frame_bb, id );
        bool temp_input_is_active = temp_input_allowed && ImGui::TempInputIsActive( id );
        if ( !temp_input_is_active )
        {
            const bool input_requested_by_tabbing =
                temp_input_allowed && ( g.LastItemData.StatusFlags & ImGuiItemStatusFlags_FocusedByTabbing ) != 0;
            const bool clicked = ( hovered && g.IO.MouseClicked[ 0 ] );
            if ( input_requested_by_tabbing || clicked || g.NavActivateId == id )
            {
                ImGui::SetActiveID( id, window );
                ImGui::SetFocusID( id, window );
                ImGui::FocusWindow( window );
                g.ActiveIdUsingNavDirMask |= ( 1 << ImGuiDir_Left ) | ( 1 << ImGuiDir_Right );
                if ( temp_input_allowed && ( input_requested_by_tabbing || ( clicked && g.IO.KeyCtrl ) ) )
                    temp_input_is_active = true;
            }
        }

        if ( temp_input_is_active )
        {
            // Only clamp CTRL+Click input when ImGuiSliderFlags_AlwaysClamp is set
            const bool is_clamp_input = ( flags & ImGuiSliderFlags_AlwaysClamp ) != 0;
            return ImGui::TempInputScalar(
                frame_bb,
                id,
                label,
                data_type,
                p_data,
                format,
                is_clamp_input ? p_min : NULL,
                is_clamp_input ? p_max : NULL );
        }

        // Slider behavior
        ImRect grab_bb;
        const bool value_changed =
            ImGui::SliderBehavior( frame_bb, id, data_type, p_data, p_min, p_max, format, flags, &grab_bb );
        if ( value_changed )
            ImGui::MarkItemEdited( id );

        // Display value using user-provided display format so user can add prefix/suffix/decorations to the value.
        char value_buf[ 64 ];
        const char* value_buf_end =
            value_buf + ImGui::DataTypeFormatString( value_buf, IM_ARRAYSIZE( value_buf ), data_type, p_data, format );

        window->DrawList->AddRectFilled(
            { frame_bb.Min.x, frame_bb.Min.y + 19 }, { frame_bb.Max.x, frame_bb.Max.y - 7 }, ImColor( 59, 66, 82 ), 2.f );
        window->DrawList->AddText( ImVec2( frame_bb.Min.x, frame_bb.Min.y ), ImColor( 255, 255, 255 ), label );
        window->DrawList->AddRectFilled(
            { frame_bb.Min.x, frame_bb.Min.y + 19 }, { grab_bb.Max.x, frame_bb.Max.y - 7 }, ImColor( 216, 222, 233 ), 2.f );

        auto HealthInfo = [ value_buf, type ] { return std::string( value_buf ) + " " + std::string( type ); };

        // X - StringSize.x / 2
        window->DrawList->AddText(
            ImVec2( frame_bb.Max.x - 25 - ImGui::CalcTextSize( HealthInfo().c_str() ).x / 2, frame_bb.Min.y ),
            ImColor( 255, 255, 255 ),
            HealthInfo().c_str() );

        IMGUI_TEST_ENGINE_ITEM_INFO( id, label, g.LastItemData.StatusFlags );
        return value_changed;
    }

    bool SliderInt(
        const char* label,
        const char* type,
        int* v,
        int v_min,
        int v_max,
        const char* format,
        ImGuiSliderFlags flags )
    {
        return SliderScalar( label, type, ImGuiDataType_S32, v, &v_min, &v_max, format, flags );
    }

    bool SliderFloat(
        const char* label,
        const char* type,
        float* v,
        float v_min,
        float v_max,
        const char* format,
        ImGuiSliderFlags flags )
    {
        return SliderScalar( label, type, ImGuiDataType_Float, v, &v_min, &v_max, format, flags );
    }

    // Getter for the old Combo() API: const char*[]
    static bool Items_ArrayGetter( void* data, int idx, const char** out_text )
    {
        const char* const* items = ( const char* const* )data;
        if ( out_text )
            *out_text = items[ idx ];
        return true;
    }

    // Getter for the old Combo() API: "item1\0item2\0item3\0"
    static bool Items_SingleStringGetter( void* data, int idx, const char** out_text )
    {
        // FIXME-OPT: we could pre-compute the indices to fasten this. But only 1 active combo means the waste is limited.
        const char* items_separated_by_zeros = ( const char* )data;
        int items_count = 0;
        const char* p = items_separated_by_zeros;
        while ( *p )
        {
            if ( idx == items_count )
                break;
            p += strlen( p ) + 1;
            items_count++;
        }
        if ( !*p )
            return false;
        if ( out_text )
            *out_text = p;
        return true;
    }

    static float CalcMaxPopupHeightFromItemCount( int items_count )
    {
        ImGuiContext& g = *GImGui;
        if ( items_count <= 0 )
            return FLT_MAX;
        return ( g.FontSize + g.Style.ItemSpacing.y ) * items_count - g.Style.ItemSpacing.y +
               ( g.Style.WindowPadding.y * 2 );
    }

    struct combo_anim
    {
        int active_outline_anim;
        float active_circle_anim;
        float open_anim;
    };

    struct selectable_anim
    {
        int active_text_anim;
        int hovered_text_anim;
    };

    bool Selectable1( const char* label, bool selected, ImGuiSelectableFlags flags, const ImVec2& size_arg )
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if ( window->SkipItems )
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;

        // Submit label or explicit size to ItemSize(), whereas ItemAdd() will submit a larger/spanning rectangle.
        ImGuiID id = window->GetID( label );
        ImVec2 label_size = ImGui::CalcTextSize( label, NULL, true );
        ImVec2 size( size_arg.x != 0.0f ? size_arg.x : label_size.x, size_arg.y != 0.0f ? size_arg.y : label_size.y );
        ImVec2 pos = window->DC.CursorPos;
        pos.y += window->DC.CurrLineTextBaseOffset;
        ImGui::ItemSize( size, 0.0f );

        // Fill horizontal space
        // We don't support (size < 0.0f) in Selectable() because the ItemSpacing extension would make explicitly
        // right-aligned sizes not visibly match other widgets.
        const bool span_all_columns = ( flags & ImGuiSelectableFlags_SpanAllColumns ) != 0;
        const float min_x = span_all_columns ? window->ParentWorkRect.Min.x : pos.x;
        const float max_x = span_all_columns ? window->ParentWorkRect.Max.x : window->WorkRect.Max.x;
        if ( size_arg.x == 0.0f || ( flags & ImGuiSelectableFlags_SpanAvailWidth ) )
            size.x = ImMax( label_size.x, max_x - min_x );

        // Text stays at the submission position, but bounding box may be extended on both sides
        const ImVec2 text_min = pos;
        const ImVec2 text_max( min_x + size.x, pos.y + size.y );

        // Selectables are meant to be tightly packed together with no click-gap, so we extend their box to cover spacing
        // between selectable.
        ImRect bb( min_x, pos.y, text_max.x, text_max.y );
        if ( ( flags & ImGuiSelectableFlags_NoPadWithHalfSpacing ) == 0 )
        {
            const float spacing_x = span_all_columns ? 0.0f : style.ItemSpacing.x;
            const float spacing_y = style.ItemSpacing.y;
            const float spacing_L = IM_FLOOR( spacing_x * 0.50f );
            const float spacing_U = IM_FLOOR( spacing_y * 0.50f );
            bb.Min.x -= spacing_L;
            bb.Min.y -= spacing_U;
            bb.Max.x += ( spacing_x - spacing_L );
            bb.Max.y += ( spacing_y - spacing_U );
        }
        // if (g.IO.KeyCtrl) { GetForegroundDrawList()->AddRect(bb.Min, bb.Max, IM_COL32(0, 255, 0, 255)); }

        // Modify ClipRect for the ItemAdd(), faster than doing a PushColumnsBackground/PushTableBackground for every
        // Selectable..
        const float backup_clip_rect_min_x = window->ClipRect.Min.x;
        const float backup_clip_rect_max_x = window->ClipRect.Max.x;
        if ( span_all_columns )
        {
            window->ClipRect.Min.x = window->ParentWorkRect.Min.x;
            window->ClipRect.Max.x = window->ParentWorkRect.Max.x;
        }

        const bool disabled_item = ( flags & ImGuiSelectableFlags_Disabled ) != 0;
        const bool item_add = ImGui::ItemAdd( bb, id, NULL, disabled_item ? ImGuiItemFlags_Disabled : ImGuiItemFlags_None );
        if ( span_all_columns )
        {
            window->ClipRect.Min.x = backup_clip_rect_min_x;
            window->ClipRect.Max.x = backup_clip_rect_max_x;
        }

        if ( !item_add )
            return false;

        const bool disabled_global = ( g.CurrentItemFlags & ImGuiItemFlags_Disabled ) != 0;
        if ( disabled_item && !disabled_global )  // Only testing this as an optimization
            ImGui::BeginDisabled();

        // FIXME: We can standardize the behavior of those two, we could also keep the fast path of override ClipRect + full
        // push on render only, which would be advantageous since most selectable are not selected.
        if ( span_all_columns && window->DC.CurrentColumns )
            ImGui::PushColumnsBackground();
        else if ( span_all_columns && g.CurrentTable )
            ImGui::TablePushBackgroundChannel();

        // We use NoHoldingActiveID on menus so user can click and _hold_ on a menu then drag to browse child entries
        ImGuiButtonFlags button_flags = 0;
        if ( flags & ImGuiSelectableFlags_NoHoldingActiveID )
        {
            button_flags |= ImGuiButtonFlags_NoHoldingActiveId;
        }
        if ( flags & ImGuiSelectableFlags_SelectOnClick )
        {
            button_flags |= ImGuiButtonFlags_PressedOnClick;
        }
        if ( flags & ImGuiSelectableFlags_SelectOnRelease )
        {
            button_flags |= ImGuiButtonFlags_PressedOnRelease;
        }
        if ( flags & ImGuiSelectableFlags_AllowDoubleClick )
        {
            button_flags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick;
        }
        if ( flags & ImGuiSelectableFlags_AllowItemOverlap )
        {
            button_flags |= ImGuiButtonFlags_AllowItemOverlap;
        }

        const bool was_selected = selected;
        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior( bb, id, &hovered, &held, button_flags );

        // Auto-select when moved into
        // - This will be more fully fleshed in the range-select branch
        // - This is not exposed as it won't nicely work with some user side handling of shift/control
        // - We cannot do 'if (g.NavJustMovedToId != id) { selected = false; pressed = was_selected; }' for two reasons
        //   - (1) it would require focus scope to be set, need exposing PushFocusScope() or equivalent (e.g.
        //   BeginSelection() calling PushFocusScope())
        //   - (2) usage will fail with clipped items
        //   The multi-select API aim to fix those issues, e.g. may be replaced with a BeginSelection() API.
        if ( ( flags & ImGuiSelectableFlags_SelectOnNav ) && g.NavJustMovedToId != 0 &&
             g.NavJustMovedToFocusScopeId == window->DC.NavFocusScopeIdCurrent )
            if ( g.NavJustMovedToId == id )
                selected = pressed = true;

        // Update NavId when clicking or when Hovering (this doesn't happen on most widgets), so navigation can be resumed
        // with gamepad/keyboard
        if ( pressed || ( hovered && ( flags & ImGuiSelectableFlags_SetNavIdOnHover ) ) )
        {
            if ( !g.NavDisableMouseHover && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent )
            {
                ImGui::SetNavID(
                    id,
                    window->DC.NavLayerCurrent,
                    window->DC.NavFocusScopeIdCurrent,
                    ImGui::WindowRectAbsToRel( window, bb ) );  // (bb == NavRect)
                g.NavDisableHighlight = true;
            }
        }
        if ( pressed )
            ImGui::MarkItemEdited( id );

        if ( flags & ImGuiSelectableFlags_AllowItemOverlap )
            ImGui::SetItemAllowOverlap();

        // In this branch, Selectable() cannot toggle the selection so this will never trigger.
        if ( selected != was_selected )  //-V547
            g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_ToggledSelection;

        // Render
        if ( held && ( flags & ImGuiSelectableFlags_DrawHoveredWhenHeld ) )
            hovered = true;

        if ( span_all_columns && window->DC.CurrentColumns )
            ImGui::PopColumnsBackground();
        else if ( span_all_columns && g.CurrentTable )
            ImGui::TablePopBackgroundChannel();

        static std::map< ImGuiID, selectable_anim > anim;
        auto it_anim = anim.find( id );
        if ( it_anim == anim.end() )
        {
            anim.insert( { id, { 0, 0 } } );
            it_anim = anim.find( id );
        }

        if ( selected )
            it_anim->second.active_text_anim += 8 * ( 1.f - ImGui::GetIO().DeltaTime );
        else
            it_anim->second.active_text_anim -= 8 * ( 1.f - ImGui::GetIO().DeltaTime );

        if ( it_anim->second.active_text_anim > 100 )
            it_anim->second.active_text_anim = 100;
        else if ( it_anim->second.active_text_anim < 0 )
            it_anim->second.active_text_anim = 0;

        if ( hovered )
            it_anim->second.hovered_text_anim += 6 * ( 1.f - ImGui::GetIO().DeltaTime );
        else
            it_anim->second.hovered_text_anim -= 6 * ( 1.f - ImGui::GetIO().DeltaTime );

        if ( it_anim->second.hovered_text_anim > 50 )
            it_anim->second.hovered_text_anim = 50;
        else if ( it_anim->second.hovered_text_anim < 0 )
            it_anim->second.hovered_text_anim = 0;

        window->DrawList->AddText(
            text_min + ImVec2( 0, 1 ),
            ImColor( 255, 255, 255, 105 + it_anim->second.active_text_anim + it_anim->second.hovered_text_anim ),
            label );

        // Automatically close popups
        if ( pressed && ( window->Flags & ImGuiWindowFlags_Popup ) && !( flags & ImGuiSelectableFlags_DontClosePopups ) &&
             !( g.LastItemData.InFlags & ImGuiItemFlags_SelectableDontClosePopup ) )
            ImGui::CloseCurrentPopup();

        if ( disabled_item && !disabled_global )
            ImGui::EndDisabled();

        IMGUI_TEST_ENGINE_ITEM_INFO( id, label, g.LastItemData.StatusFlags );
        return pressed;  //-V1020
    }

    bool BeginCombo1( const char* label, const char* preview_value, ImGuiComboFlags flags )
    {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = ImGui::GetCurrentWindow();

        ImGuiNextWindowDataFlags backup_next_window_data_flags = g.NextWindowData.Flags;
        g.NextWindowData.ClearFlags();  // We behave like Begin() and need to consume those values
        if ( window->SkipItems )
            return false;

        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID( label );
        IM_ASSERT(
            ( flags & ( ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_NoPreview ) ) !=
            ( ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_NoPreview ) );  // Can't use both flags together
        const float size = ImGui::GetWindowWidth() - 10;

        const ImRect rect(
            ImVec2( window->DC.CursorPos.x + 10, window->DC.CursorPos.y ), window->DC.CursorPos + ImVec2( size, 70 ) );
        const ImRect clickable( window->DC.CursorPos + ImVec2( 10, 18 ), window->DC.CursorPos + ImVec2( size, 44 ) );
        ImGui::ItemSize( rect, style.FramePadding.y );
        if ( !ImGui::ItemAdd( clickable, id, &rect ) )
            return false;

        // Open on click
        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior( clickable, id, &hovered, &held );
        const ImGuiID popup_id = ImHashStr( "##ComboPopup", 0, id );
        bool popup_open = ImGui::IsPopupOpen( popup_id, ImGuiPopupFlags_None );
        if ( pressed && !popup_open )
        {
            ImGui::OpenPopupEx( popup_id, ImGuiPopupFlags_None );
            popup_open = true;
        }

        static std::map< ImGuiID, combo_anim > anim;
        auto it_anim = anim.find( id );
        if ( it_anim == anim.end() )
        {
            anim.insert( { id, { 0, 0.f, 0.f } } );
            it_anim = anim.find( id );
        }

        if ( popup_open )
            it_anim->second.active_outline_anim += 4 * ( 1.f - ImGui::GetIO().DeltaTime );
        else
            it_anim->second.active_outline_anim -= 4 * ( 1.f - ImGui::GetIO().DeltaTime );

        if ( it_anim->second.active_outline_anim > 20 )
            it_anim->second.active_outline_anim = 20;
        else if ( it_anim->second.active_outline_anim < 0 )
            it_anim->second.active_outline_anim = 0;

        if ( popup_open )
            it_anim->second.active_circle_anim += 0.2f * ( 1.f - ImGui::GetIO().DeltaTime );
        else
            it_anim->second.active_circle_anim -= 0.2f * ( 1.f - ImGui::GetIO().DeltaTime );

        if ( it_anim->second.active_circle_anim > 0.8f )
            it_anim->second.active_circle_anim = 0.8f;
        else if ( it_anim->second.active_circle_anim < 0 )
            it_anim->second.active_circle_anim = 0;

        if ( popup_open )
            it_anim->second.open_anim += 0.05f * ( 1.f - ImGui::GetIO().DeltaTime );
        else
            it_anim->second.open_anim -= 0.05f * ( 1.f - ImGui::GetIO().DeltaTime );

        if ( it_anim->second.open_anim > 1 )
            it_anim->second.open_anim = 1;
        else if ( it_anim->second.open_anim < 0 )
            it_anim->second.open_anim = 0;

        ImGuiComboFlags flags2 = ImDrawFlags_RoundCornersTop;

        if ( !ImGui::IsPopupOpen( popup_id, ImGuiPopupFlags_None ) )
        {
            flags2 = ImDrawFlags_RoundCornersAll;
        }
        else
        {
            flags2 = ImDrawFlags_RoundCornersTop;
        }

        window->DrawList->AddRectFilled( clickable.Min, clickable.Max, ImColor( 59, 66, 82 ), 2.f, flags2 );

        ImGui::PushFont( nord::menu_mgr.widget_normal );

        window->DrawList->AddText( rect.Min, ImColor( 255, 255, 255, 255 ), label );

        ImGui::PopFont();

        ImGui::PushFont( nord::menu_mgr.widget_small );

        window->DrawList->PushClipRect( clickable.Min, clickable.Max - ImVec2( 29, 0 ) );
        window->DrawList->AddText( clickable.Min + ImVec2( 5, 7 ), ImColor( 255, 255, 255, 255 ), preview_value );
        window->DrawList->PopClipRect();

        ImGui::PopFont();

        if ( !popup_open )
            return false;

        g.NextWindowData.Flags = backup_next_window_data_flags;
        if ( !ImGui::IsPopupOpen( popup_id, ImGuiPopupFlags_None ) )
        {
            g.NextWindowData.ClearFlags();
            return false;
        }

        // Set popup size
        float w = clickable.GetWidth();
        if ( g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSizeConstraint )
        {
            g.NextWindowData.SizeConstraintRect.Min.x = ImMax( g.NextWindowData.SizeConstraintRect.Min.x, w );
        }
        else
        {
            if ( ( flags & ImGuiComboFlags_HeightMask_ ) == 0 )
                flags |= ImGuiComboFlags_HeightRegular;
            IM_ASSERT( ImIsPowerOfTwo( flags & ImGuiComboFlags_HeightMask_ ) );  // Only one
            int popup_max_height_in_items = -1;
            if ( flags & ImGuiComboFlags_HeightRegular )
                popup_max_height_in_items = 8;
            else if ( flags & ImGuiComboFlags_HeightSmall )
                popup_max_height_in_items = 4;
            else if ( flags & ImGuiComboFlags_HeightLarge )
                popup_max_height_in_items = 20;
            ImGui::SetNextWindowSizeConstraints(
                ImVec2( w, 0.0f ),
                ImVec2(
                    FLT_MAX, CalcMaxPopupHeightFromItemCount( popup_max_height_in_items ) * it_anim->second.open_anim ) );
        }

        // This is essentially a specialized version of BeginPopupEx()
        char name[ 16 ];
        ImFormatString(
            name, IM_ARRAYSIZE( name ), "##Combo_%02d", g.BeginPopupStack.Size );  // Recycle windows based on depth

        // Set position given a custom constraint (peak into expected window size so we can position it)
        // FIXME: This might be easier to express with an hypothetical SetNextWindowPosConstraints() function?
        // FIXME: This might be moved to Begin() or at least around the same spot where Tooltips and other Popups are calling
        // FindBestWindowPosForPopupEx()?
        if ( ImGuiWindow* popup_window = ImGui::FindWindowByName( name ) )
            if ( popup_window->WasActive )
            {
                // Always override 'AutoPosLastDirection' to not leave a chance for a past value to affect us.
                ImVec2 size_expected = ImGui::CalcWindowNextAutoFitSize( popup_window );
                popup_window->AutoPosLastDirection =
                    ( flags & ImGuiComboFlags_PopupAlignLeft )
                        ? ImGuiDir_Left
                        : ImGuiDir_Down;  // Left = "Below, Toward Left", Down = "Below, Toward Right (default)"
                ImRect r_outer = ImGui::GetPopupAllowedExtentRect( popup_window );
                ImVec2 pos = ImGui::FindBestWindowPosForPopupEx(
                    clickable.GetBL(),
                    size_expected,
                    &popup_window->AutoPosLastDirection,
                    r_outer,
                    clickable,
                    ImGuiPopupPositionPolicy_ComboBox );
                ImGui::SetNextWindowPos( pos + ImVec2( 0, 2 ) );
            }

        // We don't use BeginPopupEx() solely because we have a custom name string, which we could make an argument to
        // BeginPopupEx()
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Popup |
                                        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove;
        ImGui::PushStyleVar(
            ImGuiStyleVar_WindowPadding, ImVec2( 5, 5 ) );  // Horizontally align ourselves with the framed text
        ImGui::PushStyleVar( ImGuiStyleVar_PopupBorderSize, 1.f );
        ImGui::PushStyleVar( ImGuiStyleVar_PopupRounding, 0.f );
        ImGui::PushStyleColor( ImGuiCol_PopupBg, ImVec4( ImColor( 59, 66, 82 ) ) );
        // PushStyleColor(ImGuiCol_Border, ImVec4(ImColor(255, 255, 255, (15 + it_anim->second.active_outline_anim))));
        bool ret = ImGui::Begin( name, NULL, window_flags );
        ImGui::PopStyleVar( 3 );
        ImGui::PopStyleColor();
        if ( !ret )
        {
            ImGui::EndPopup();
            IM_ASSERT( 0 );  // This should never happen as we tested for IsPopupOpen() above
            return false;
        }
        return true;
    }

    bool ComboA(
        const char* label,
        int* current_item,
        bool ( *items_getter )( void*, int, const char** ),
        void* data,
        int items_count,
        int popup_max_height_in_items )
    {
        ImGuiContext& g = *GImGui;

        // Call the getter to obtain the preview string which is a parameter to BeginCombo()
        const char* preview_value = NULL;
        if ( *current_item >= 0 && *current_item < items_count )
            items_getter( data, *current_item, &preview_value );

        // The old Combo() API exposed "popup_max_height_in_items". The new more general BeginCombo() API doesn't have/need
        // it, but we emulate it here.
        if ( popup_max_height_in_items != -1 && !( g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSizeConstraint ) )
            ImGui::SetNextWindowSizeConstraints(
                ImVec2( 0, 0 ), ImVec2( FLT_MAX, CalcMaxPopupHeightFromItemCount( popup_max_height_in_items ) ) );

        if ( !BeginCombo1( label, preview_value, ImGuiComboFlags_None ) )
            return false;

        // Display items
        // FIXME-OPT: Use clipper (but we need to disable it on the appearing frame to make sure our call to
        // SetItemDefaultFocus() is processed)
        bool value_changed = false;
        for ( int i = 0; i < items_count; i++ )
        {
            ImGui::PushID( i );
            const bool item_selected = ( i == *current_item );
            const char* item_text;
            if ( !items_getter( data, i, &item_text ) )
                item_text = "*Unknown item*";
            if ( Selectable1( item_text, item_selected, 0, ImVec2( 0, 0 ) ) )
            {
                value_changed = true;
                *current_item = i;
            }
            if ( item_selected )
                ImGui::SetItemDefaultFocus();
            ImGui::PopID();
        }

        ImGui::EndCombo();

        if ( value_changed )
            ImGui::MarkItemEdited( g.LastItemData.ID );

        return value_changed;
    }

    // Combo box helper allowing to pass an array of strings.
    bool Combo( const char* label, int* current_item, const char* const items[], int items_count, int height_in_items )
    {
        const bool value_changed =
            ComboA( label, current_item, Items_ArrayGetter, ( void* )items, items_count, height_in_items );
        return value_changed;
    }

    static void ColorEditRestoreHS( const float* col, float* H, float* S, float* V )
    {
        // This check is optional. Suppose we have two color widgets side by side, both widgets display different colors, but
        // both colors have hue and/or saturation undefined. With color check: hue/saturation is preserved in one widget.
        // Editing color in one widget would reset hue/saturation in another one. Without color check: common hue/saturation
        // would be displayed in all widgets that have hue/saturation undefined. g.ColorEditLastColor is stored as ImU32 RGB
        // value: this essentially gives us color equality check with reduced precision. Tiny external color changes would
        // not be detected and this check would still pass. This is OK, since we only restore hue/saturation _only_ if they
        // are undefined, therefore this change flipping hue/saturation from undefined to a very tiny value would still be
        // represented in color picker.
        ImGuiContext& g = *GImGui;
        // When S == 0, H is undefined.
        // When H == 1 it wraps around to 0.
        if ( *S == 0.0f || ( *H == 0.0f && g.ColorEditLastHue == 1 ) )
            *H = g.ColorEditLastHue;

        // When V == 0, S is undefined.
        if ( *V == 0.0f )
            *S = g.ColorEditLastSat;
    }

    static void RenderArrowsForVerticalBar( ImDrawList* draw_list, ImVec2 pos, ImVec2 half_sz, float bar_w, float alpha )
    {
        ImU32 alpha8 = IM_F32_TO_INT8_SAT( alpha );
        ImGui::RenderArrowPointingAt(
            draw_list,
            ImVec2( pos.x + half_sz.x + 1, pos.y ),
            ImVec2( half_sz.x + 2, half_sz.y + 1 ),
            ImGuiDir_Right,
            IM_COL32( 0, 0, 0, alpha8 ) );
        ImGui::RenderArrowPointingAt(
            draw_list, ImVec2( pos.x + half_sz.x, pos.y ), half_sz, ImGuiDir_Right, IM_COL32( 255, 255, 255, alpha8 ) );
        ImGui::RenderArrowPointingAt(
            draw_list,
            ImVec2( pos.x + bar_w - half_sz.x - 1, pos.y ),
            ImVec2( half_sz.x + 2, half_sz.y + 1 ),
            ImGuiDir_Left,
            IM_COL32( 0, 0, 0, alpha8 ) );
        ImGui::RenderArrowPointingAt(
            draw_list,
            ImVec2( pos.x + bar_w - half_sz.x, pos.y ),
            half_sz,
            ImGuiDir_Left,
            IM_COL32( 255, 255, 255, alpha8 ) );
    }

    bool color_picker( const char* label, float col[ 4 ], ImGuiColorEditFlags flags, const float* ref_col )
    {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if ( window->SkipItems )
            return false;

        ImDrawList* draw_list = window->DrawList;
        ImGuiStyle& style = g.Style;
        ImGuiIO& io = g.IO;

        const float width = ImGui::CalcItemWidth();
        g.NextItemData.ClearFlags();

        ImGui::PushID( label );
        ImGui::BeginGroup();

        if ( !( flags & ImGuiColorEditFlags_NoSidePreview ) )
            flags |= ImGuiColorEditFlags_NoSmallPreview;

        // Context menu: display and store options.
        if ( !( flags & ImGuiColorEditFlags_NoOptions ) )
            ImGui::ColorPickerOptionsPopup( col, flags );

        // Read stored options
        if ( !( flags & ImGuiColorEditFlags_PickerMask_ ) )
            flags |= ( ( g.ColorEditOptions & ImGuiColorEditFlags_PickerMask_ ) ? g.ColorEditOptions
                                                                                : ImGuiColorEditFlags_DefaultOptions_ ) &
                     ImGuiColorEditFlags_PickerMask_;
        if ( !( flags & ImGuiColorEditFlags_InputMask_ ) )
            flags |= ( ( g.ColorEditOptions & ImGuiColorEditFlags_InputMask_ ) ? g.ColorEditOptions
                                                                               : ImGuiColorEditFlags_DefaultOptions_ ) &
                     ImGuiColorEditFlags_InputMask_;
        IM_ASSERT( ImIsPowerOfTwo( flags & ImGuiColorEditFlags_PickerMask_ ) );  // Check that only 1 is selected
        IM_ASSERT( ImIsPowerOfTwo( flags & ImGuiColorEditFlags_InputMask_ ) );   // Check that only 1 is selected
        if ( !( flags & ImGuiColorEditFlags_NoOptions ) )
            flags |= ( g.ColorEditOptions & ImGuiColorEditFlags_AlphaBar );

        // Setup
        int components = ( flags & ImGuiColorEditFlags_NoAlpha ) ? 3 : 4;
        bool alpha_bar = ( flags & ImGuiColorEditFlags_AlphaBar ) && !( flags & ImGuiColorEditFlags_NoAlpha );
        ImVec2 picker_pos = window->DC.CursorPos;
        float square_sz = ImGui::GetFrameHeight();
        float bars_width = square_sz;  // Arbitrary smallish width of Hue/Alpha picking bars
        float sv_picker_size = ImMax(
            bars_width * 1,
            width - ( alpha_bar ? 2 : 1 ) * ( bars_width + style.ItemInnerSpacing.x ) );  // Saturation/Value picking box
        float bar0_pos_x = picker_pos.x + sv_picker_size + style.ItemInnerSpacing.x;
        float bar1_pos_x = bar0_pos_x + bars_width + style.ItemInnerSpacing.x;
        float bars_triangles_half_sz = IM_FLOOR( bars_width * 0.20f );

        float backup_initial_col[ 4 ];
        memcpy( backup_initial_col, col, components * sizeof( float ) );

        float wheel_thickness = sv_picker_size * 0.08f;
        float wheel_r_outer = sv_picker_size * 0.50f;
        float wheel_r_inner = wheel_r_outer - wheel_thickness;
        ImVec2 wheel_center( picker_pos.x + ( sv_picker_size + bars_width ) * 0.5f, picker_pos.y + sv_picker_size * 0.5f );

        // Note: the triangle is displayed rotated with triangle_pa pointing to Hue, but most coordinates stays unrotated for
        // logic.
        float triangle_r = wheel_r_inner - ( int )( sv_picker_size * 0.027f );
        ImVec2 triangle_pa = ImVec2( triangle_r, 0.0f );                             // Hue point.
        ImVec2 triangle_pb = ImVec2( triangle_r * -0.5f, triangle_r * -0.866025f );  // Black point.
        ImVec2 triangle_pc = ImVec2( triangle_r * -0.5f, triangle_r * +0.866025f );  // White point.

        float H = col[ 0 ], S = col[ 1 ], V = col[ 2 ];
        float R = col[ 0 ], G = col[ 1 ], B = col[ 2 ];
        if ( flags & ImGuiColorEditFlags_InputRGB )
        {
            // Hue is lost when converting from greyscale rgb (saturation=0). Restore it.
            ImGui::ColorConvertRGBtoHSV( R, G, B, H, S, V );
            if ( g.ColorEditLastColor = ImGui::ColorConvertFloat4ToU32( ImVec4( col[ 0 ], col[ 1 ], col[ 2 ], 0 ) ) == 0 )
            {
                if ( S == 0 )
                    H = g.ColorEditLastHue;
                if ( V == 0 )
                    S = g.ColorEditLastSat;
            }
        }
        else if ( flags & ImGuiColorEditFlags_InputHSV )
        {
            ImGui::ColorConvertHSVtoRGB( H, S, V, R, G, B );
        }

        bool value_changed = false, value_changed_h = false, value_changed_sv = false;

        ImGui::PushItemFlag( ImGuiItemFlags_NoNav, true );
        if ( flags & ImGuiColorEditFlags_PickerHueWheel )
        {
            // Hue wheel + SV triangle logic
            ImGui::InvisibleButton(
                "hsv", ImVec2( sv_picker_size + style.ItemInnerSpacing.x + bars_width, sv_picker_size ) );
            if ( ImGui::IsItemActive() )
            {
                ImVec2 initial_off = g.IO.MouseClickedPos[ 0 ] - wheel_center;
                ImVec2 current_off = g.IO.MousePos - wheel_center;
                float initial_dist2 = ImLengthSqr( initial_off );
                if ( initial_dist2 >= ( wheel_r_inner - 1 ) * ( wheel_r_inner - 1 ) &&
                     initial_dist2 <= ( wheel_r_outer + 1 ) * ( wheel_r_outer + 1 ) )
                {
                    // Interactive with Hue wheel
                    H = ImAtan2( current_off.y, current_off.x ) / IM_PI * 0.5f;
                    if ( H < 0.0f )
                        H += 1.0f;
                    value_changed = value_changed_h = true;
                }
                float cos_hue_angle = ImCos( -H * 2.0f * IM_PI );
                float sin_hue_angle = ImSin( -H * 2.0f * IM_PI );
                if ( ImTriangleContainsPoint(
                         triangle_pa, triangle_pb, triangle_pc, ImRotate( initial_off, cos_hue_angle, sin_hue_angle ) ) )
                {
                    // Interacting with SV triangle
                    ImVec2 current_off_unrotated = ImRotate( current_off, cos_hue_angle, sin_hue_angle );
                    if ( !ImTriangleContainsPoint( triangle_pa, triangle_pb, triangle_pc, current_off_unrotated ) )
                        current_off_unrotated =
                            ImTriangleClosestPoint( triangle_pa, triangle_pb, triangle_pc, current_off_unrotated );
                    float uu, vv, ww;
                    ImTriangleBarycentricCoords( triangle_pa, triangle_pb, triangle_pc, current_off_unrotated, uu, vv, ww );
                    V = ImClamp( 1.0f - vv, 0.0001f, 1.0f );
                    S = ImClamp( uu / V, 0.0001f, 1.0f );
                    value_changed = value_changed_sv = true;
                }
            }
            if ( !( flags & ImGuiColorEditFlags_NoOptions ) )
                ImGui::OpenPopupOnItemClick( "context" );
        }
        else if ( flags & ImGuiColorEditFlags_PickerHueBar )
        {
            // SV rectangle logic
            ImGui::InvisibleButton( "sv", ImVec2( sv_picker_size, sv_picker_size ) );
            if ( ImGui::IsItemActive() )
            {
                S = ImSaturate( ( io.MousePos.x - picker_pos.x ) / ( sv_picker_size - 1 ) );
                V = 1.0f - ImSaturate( ( io.MousePos.y - picker_pos.y ) / ( sv_picker_size - 1 ) );
                value_changed = value_changed_sv = true;
            }
            if ( !( flags & ImGuiColorEditFlags_NoOptions ) )
                ImGui::OpenPopupOnItemClick( "context" );

            // Hue bar logic
            ImGui::SetCursorScreenPos( ImVec2( bar0_pos_x, picker_pos.y ) );
            ImGui::InvisibleButton( "hue", ImVec2( bars_width, sv_picker_size ) );
            if ( ImGui::IsItemActive() )
            {
                H = ImSaturate( ( io.MousePos.y - picker_pos.y ) / ( sv_picker_size - 1 ) );
                value_changed = value_changed_h = true;
            }
        }

        // Alpha bar logic
        if ( alpha_bar )
        {
            ImGui::SetCursorScreenPos( ImVec2( bar1_pos_x, picker_pos.y ) );
            ImGui::InvisibleButton( "alpha", ImVec2( bars_width, sv_picker_size ) );
            if ( ImGui::IsItemActive() )
            {
                col[ 3 ] = 1.0f - ImSaturate( ( io.MousePos.y - picker_pos.y ) / ( sv_picker_size - 1 ) );
                value_changed = true;
            }
        }
        ImGui::PopItemFlag();  // ImGuiItemFlags_NoNav

        if ( !( flags & ImGuiColorEditFlags_NoSidePreview ) )
        {
            ImGui::SameLine( 0, style.ItemInnerSpacing.x );
            ImGui::BeginGroup();
        }

        if ( !( flags & ImGuiColorEditFlags_NoLabel ) )
        {
            const char* label_display_end = ImGui::FindRenderedTextEnd( label );
            if ( label != label_display_end )
            {
                if ( ( flags & ImGuiColorEditFlags_NoSidePreview ) )
                    ImGui::SameLine( 0, style.ItemInnerSpacing.x );
                ImGui::TextEx( label, label_display_end );
            }
        }

        // Convert back color to RGB
        if ( value_changed_h || value_changed_sv )
        {
            if ( flags & ImGuiColorEditFlags_InputRGB )
            {
                ImGui::ColorConvertHSVtoRGB(
                    H >= 1.0f ? H - 10 * 1e-6f : H,
                    S > 0.0f ? S : 10 * 1e-6f,
                    V > 0.0f ? V : 1e-6f,
                    col[ 0 ],
                    col[ 1 ],
                    col[ 2 ] );
                g.ColorEditLastHue = H;
                g.ColorEditLastSat = S;
                g.ColorEditLastColor = ImGui::ColorConvertFloat4ToU32( ImVec4( col[ 0 ], col[ 1 ], col[ 2 ], 0 ) );
            }
            else if ( flags & ImGuiColorEditFlags_InputHSV )
            {
                col[ 0 ] = H;
                col[ 1 ] = S;
                col[ 2 ] = V;
            }
        }

        // R,G,B and H,S,V slider color editor
        bool value_changed_fix_hue_wrap = false;
        // ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(30 / 255.f, 30 / 255.f, 30 / 255.f, 1.f));
        // ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(30 / 255.f, 30 / 255.f, 30 / 255.f, 1.f));
        // ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(30 / 255.f, 30 / 255.f, 30 / 255.f, 1.f));
        // ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, ImVec4(40 / 255.f, 40 / 255.f, 40 / 255.f, 1.f));

        // if ((flags & ImGuiColorEditFlags_NoInputs) == 0)
        //{
        //	ImGui::PushItemWidth((alpha_bar ? bar1_pos_x : bar0_pos_x) + bars_width - picker_pos.x);
        //	ImGuiColorEditFlags sub_flags_to_forward = ImGuiColorEditFlags__DataTypeMask | ImGuiColorEditFlags__InputMask
        //| ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoOptions |
        // ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaPreviewHalf;
        //	ImGuiColorEditFlags sub_flags = (flags & sub_flags_to_forward) | ImGuiColorEditFlags_NoPicker;
        //	if (flags & ImGuiColorEditFlags_DisplayRGB || (flags & ImGuiColorEditFlags__DisplayMask) == 0)
        //		if (ImGui::ColorEdit4("##rgb", col, sub_flags | ImGuiColorEditFlags_DisplayRGB))
        //		{
        //			// FIXME: Hackily differentiating using the DragInt (ActiveId != 0 && !ActiveIdAllowOverlap)
        // vs. using the InputText or DropTarget.
        //			// For the later we don't want to run the hue-wrap canceling code. If you are well versed in
        // HSV picker please provide your input! (See #2050) 			value_changed_fix_hue_wrap = (g.ActiveId != 0
        // &&
        //! g.ActiveIdAllowOverlap); 			value_changed = true;
        //		}
        //	if (flags & ImGuiColorEditFlags_DisplayHex || (flags & ImGuiColorEditFlags__DisplayMask) == 0)
        //		value_changed |= ImGui::ColorEdit4("##hex", col, sub_flags | ImGuiColorEditFlags_DisplayHex);
        //	ImGui::PopItemWidth();
        //}

        // ImGui::PopStyleColor(3);

        // Try to cancel hue wrap (after ColorEdit4 call), if any
        if ( value_changed_fix_hue_wrap && ( flags & ImGuiColorEditFlags_InputRGB ) )
        {
            float new_H, new_S, new_V;
            ImGui::ColorConvertRGBtoHSV( col[ 0 ], col[ 1 ], col[ 2 ], new_H, new_S, new_V );
            if ( new_H <= 0 && H > 0 )
            {
                if ( new_V <= 0 && V != new_V )
                    ImGui::ColorConvertHSVtoRGB( H, S, new_V <= 0 ? V * 0.5f : new_V, col[ 0 ], col[ 1 ], col[ 2 ] );
                else if ( new_S <= 0 )
                    ImGui::ColorConvertHSVtoRGB( H, new_S <= 0 ? S * 0.5f : new_S, new_V, col[ 0 ], col[ 1 ], col[ 2 ] );
            }
        }

        if ( value_changed )
        {
            if ( flags & ImGuiColorEditFlags_InputRGB )
            {
                R = col[ 0 ];
                G = col[ 1 ];
                B = col[ 2 ];
                ImGui::ColorConvertRGBtoHSV( R, G, B, H, S, V );
                if ( g.ColorEditLastColor = ImGui::ColorConvertFloat4ToU32( ImVec4( col[ 0 ], col[ 1 ], col[ 2 ], 0 ) ) ==
                                            0 )  // Fix local Hue as display below will use it immediately.
                {
                    if ( S == 0 )
                        H = g.ColorEditLastHue;
                    if ( V == 0 )
                        S = g.ColorEditLastSat;
                }
            }
            else if ( flags & ImGuiColorEditFlags_InputHSV )
            {
                H = col[ 0 ];
                S = col[ 1 ];
                V = col[ 2 ];
                ImGui::ColorConvertHSVtoRGB( H, S, V, R, G, B );
            }
        }

        const int style_alpha8 = IM_F32_TO_INT8_SAT( style.Alpha );
        const ImU32 col_black = IM_COL32( 0, 0, 0, style_alpha8 );
        const ImU32 col_white = IM_COL32( 255, 255, 255, style_alpha8 );
        const ImU32 col_midgrey = IM_COL32( 128, 128, 128, style_alpha8 );
        const ImU32 col_hues[ 6 + 1 ] = { IM_COL32( 255, 0, 0, style_alpha8 ), IM_COL32( 255, 255, 0, style_alpha8 ),
                                          IM_COL32( 0, 255, 0, style_alpha8 ), IM_COL32( 0, 255, 255, style_alpha8 ),
                                          IM_COL32( 0, 0, 255, style_alpha8 ), IM_COL32( 255, 0, 255, style_alpha8 ),
                                          IM_COL32( 255, 0, 0, style_alpha8 ) };

        ImVec4 hue_color_f( 1, 1, 1, style.Alpha );
        ImGui::ColorConvertHSVtoRGB( H, 1, 1, hue_color_f.x, hue_color_f.y, hue_color_f.z );
        ImU32 hue_color32 = ImGui::ColorConvertFloat4ToU32( hue_color_f );
        ImU32 user_col32_striped_of_alpha = ImGui::ColorConvertFloat4ToU32(
            ImVec4( R, G, B, style.Alpha ) );  // Important: this is still including the main rendering/style alpha!!

        ImVec2 sv_cursor_pos;

        if ( flags & ImGuiColorEditFlags_PickerHueWheel )
        {
            // Render Hue Wheel
            const float aeps = 0.5f / wheel_r_outer;  // Half a pixel arc length in radians (2pi cancels out).
            const int segment_per_arc = ImMax( 4, ( int )wheel_r_outer / 12 );
            for ( int n = 0; n < 6; n++ )
            {
                const float a0 = ( n ) / 6.0f * 2.0f * IM_PI - aeps;
                const float a1 = ( n + 1.0f ) / 6.0f * 2.0f * IM_PI + aeps;
                const int vert_start_idx = draw_list->VtxBuffer.Size;
                draw_list->PathArcTo( wheel_center, ( wheel_r_inner + wheel_r_outer ) * 0.5f, a0, a1, segment_per_arc );
                draw_list->PathStroke( col_white, 0, wheel_thickness );
                const int vert_end_idx = draw_list->VtxBuffer.Size;

                // Paint colors over existing vertices
                ImVec2 gradient_p0(
                    wheel_center.x + ImCos( a0 ) * wheel_r_inner, wheel_center.y + ImSin( a0 ) * wheel_r_inner );
                ImVec2 gradient_p1(
                    wheel_center.x + ImCos( a1 ) * wheel_r_inner, wheel_center.y + ImSin( a1 ) * wheel_r_inner );
                ImGui::ShadeVertsLinearColorGradientKeepAlpha(
                    draw_list, vert_start_idx, vert_end_idx, gradient_p0, gradient_p1, col_hues[ n ], col_hues[ n + 1 ] );
            }

            // Render Cursor + preview on Hue Wheel
            float cos_hue_angle = ImCos( H * 2.0f * IM_PI );
            float sin_hue_angle = ImSin( H * 2.0f * IM_PI );
            ImVec2 hue_cursor_pos(
                wheel_center.x + cos_hue_angle * ( wheel_r_inner + wheel_r_outer ) * 0.5f,
                wheel_center.y + sin_hue_angle * ( wheel_r_inner + wheel_r_outer ) * 0.5f );
            float hue_cursor_rad = value_changed_h ? wheel_thickness * 0.65f : wheel_thickness * 0.55f;
            int hue_cursor_segments = ImClamp( ( int )( hue_cursor_rad / 1.4f ), 9, 32 );
            draw_list->AddCircleFilled( hue_cursor_pos, hue_cursor_rad, hue_color32, hue_cursor_segments );
            draw_list->AddCircle( hue_cursor_pos, hue_cursor_rad + 1, col_midgrey, hue_cursor_segments );
            draw_list->AddCircle( hue_cursor_pos, hue_cursor_rad, col_white, hue_cursor_segments );

            // Render SV triangle (rotated according to hue)
            ImVec2 tra = wheel_center + ImRotate( triangle_pa, cos_hue_angle, sin_hue_angle );
            ImVec2 trb = wheel_center + ImRotate( triangle_pb, cos_hue_angle, sin_hue_angle );
            ImVec2 trc = wheel_center + ImRotate( triangle_pc, cos_hue_angle, sin_hue_angle );
            ImVec2 uv_white = ImGui::GetFontTexUvWhitePixel();
            draw_list->PrimReserve( 6, 6 );
            draw_list->PrimVtx( tra, uv_white, hue_color32 );
            draw_list->PrimVtx( trb, uv_white, hue_color32 );
            draw_list->PrimVtx( trc, uv_white, col_white );
            draw_list->PrimVtx( tra, uv_white, 0 );
            draw_list->PrimVtx( trb, uv_white, col_black );
            draw_list->PrimVtx( trc, uv_white, 0 );
            draw_list->AddTriangle( tra, trb, trc, col_midgrey, 1.5f );
            sv_cursor_pos = ImLerp( ImLerp( trc, tra, ImSaturate( S ) ), trb, ImSaturate( 1 - V ) );
        }
        else if ( flags & ImGuiColorEditFlags_PickerHueBar )
        {
            // Render SV Square
            draw_list->AddRectFilledMultiColor(
                picker_pos,
                picker_pos + ImVec2( sv_picker_size, sv_picker_size ),
                col_white,
                hue_color32,
                hue_color32,
                col_white );
            draw_list->AddRectFilledMultiColor(
                picker_pos, picker_pos + ImVec2( sv_picker_size, sv_picker_size ), 0, 0, col_black, col_black );
            ImGui::RenderFrameBorder( picker_pos, picker_pos + ImVec2( sv_picker_size, sv_picker_size ), 0.0f );
            sv_cursor_pos.x = ImClamp(
                IM_ROUND( picker_pos.x + ImSaturate( S ) * sv_picker_size ),
                picker_pos.x + 2,
                picker_pos.x + sv_picker_size - 2 );  // Sneakily prevent the circle to stick out too much
            sv_cursor_pos.y = ImClamp(
                IM_ROUND( picker_pos.y + ImSaturate( 1 - V ) * sv_picker_size ),
                picker_pos.y + 2,
                picker_pos.y + sv_picker_size - 2 );

            // Render Hue Bar
            for ( int i = 0; i < 6; ++i )
                draw_list->AddRectFilledMultiColor(
                    ImVec2( bar0_pos_x, picker_pos.y + i * ( sv_picker_size / 6 ) ),
                    ImVec2( bar0_pos_x + bars_width, picker_pos.y + ( i + 1 ) * ( sv_picker_size / 6 ) ),
                    col_hues[ i ],
                    col_hues[ i ],
                    col_hues[ i + 1 ],
                    col_hues[ i + 1 ] );
            float bar0_line_y = IM_ROUND( picker_pos.y + H * sv_picker_size );
            ImGui::RenderFrameBorder(
                ImVec2( bar0_pos_x, picker_pos.y ), ImVec2( bar0_pos_x + bars_width, picker_pos.y + sv_picker_size ), 0.0f );
            RenderArrowsForVerticalBar(
                draw_list,
                ImVec2( bar0_pos_x - 1, bar0_line_y ),
                ImVec2( bars_triangles_half_sz + 1, bars_triangles_half_sz ),
                bars_width + 2.0f,
                style.Alpha );
        }

        // Render cursor/preview circle (clamp S/V within 0..1 range because floating points colors may lead HSV values to be
        // out of range)
        float sv_cursor_rad = value_changed_sv ? 10.0f : 6.0f;
        draw_list->AddRectFilled( sv_cursor_pos - ImVec2( 2, 2 ), sv_cursor_pos + ImVec2( 2, 2 ), ImColor( 0, 0, 0 ) );
        draw_list->AddRectFilled( sv_cursor_pos - ImVec2( 1, 1 ), sv_cursor_pos + ImVec2( 1, 1 ), ImColor( 255, 255, 255 ) );

        // Render alpha bar
        if ( alpha_bar )
        {
            float alpha = ImSaturate( col[ 3 ] );
            ImRect bar1_bb( bar1_pos_x, picker_pos.y, bar1_pos_x + bars_width, picker_pos.y + sv_picker_size );
            ImGui::RenderColorRectWithAlphaCheckerboard(
                draw_list, bar1_bb.Min, bar1_bb.Max, 0, bar1_bb.GetWidth() / 2.0f, ImVec2( 0.0f, 0.0f ) );
            draw_list->AddRectFilledMultiColor(
                bar1_bb.Min,
                bar1_bb.Max,
                user_col32_striped_of_alpha,
                user_col32_striped_of_alpha,
                user_col32_striped_of_alpha & ~IM_COL32_A_MASK,
                user_col32_striped_of_alpha & ~IM_COL32_A_MASK );
            float bar1_line_y = IM_ROUND( picker_pos.y + ( 1.0f - alpha ) * sv_picker_size );
            ImGui::RenderFrameBorder( bar1_bb.Min, bar1_bb.Max, 0.0f );
            RenderArrowsForVerticalBar(
                draw_list,
                ImVec2( bar1_pos_x - 1, bar1_line_y ),
                ImVec2( bars_triangles_half_sz + 1, bars_triangles_half_sz ),
                bars_width + 2.0f,
                style.Alpha );
        }

        ImGui::Spacing();
        ImGui::Spacing();

        ImGui::EndGroup();

        if ( value_changed && memcmp( backup_initial_col, col, components * sizeof( float ) ) == 0 )
            value_changed = false;
        if ( value_changed )
            ImGui::MarkItemEdited( g.LastItemData.ID );

        ImGui::PopID();

        return value_changed;
    }
}  // namespace menu_mgr_utils

static int tab = 0;
static int tab_2 = 0;

namespace nord
{
    bool menu::create_window()
    {
        // gonna handle all the style stuff here to simplify shit

        // colors[ImGuiCol_ChildBg] = ImColor(255, 255, 255);

        ImGui::SetNextWindowSize( { 538, 542 } );
        ImGui::Begin(
            "wnd", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground );

        // body
        ImGui::GetWindowDrawList()->AddRectFilled(
            ImVec2( ImGui::GetWindowPos().x + 5, ImGui::GetWindowPos().y + 3 ),
            ImVec2( ImGui::GetWindowPos().x + 533, ImGui::GetWindowPos().y + 539 ),
            ImColor( 59, 66, 82 ),
            5.f );

        // vertical side bar
        ImGui::GetWindowDrawList()->AddRectFilled(
            ImVec2( ImGui::GetWindowPos().x + 5, ImGui::GetWindowPos().y + 3 ),
            ImVec2( ImGui::GetWindowPos().x + 70, ImGui::GetWindowPos().y + 539 ),
            ImColor( 46, 52, 64 ),
            5.f,
            ImDrawFlags_RoundCornersTopLeft | ImDrawFlags_RoundCornersBottomLeft );

        // horizontal bar
        ImGui::GetWindowDrawList()->AddRectFilled(
            ImVec2( ImGui::GetWindowPos().x + 5, ImGui::GetWindowPos().y + 3 ),
            ImVec2( ImGui::GetWindowPos().x + 533, ImGui::GetWindowPos().y + 30 ),
            ImColor( 46, 52, 64 ),
            5.f,
            ImDrawFlags_RoundCornersTop );

        // logo
        ImGui::SetCursorPosY( ImGui::GetCursorPosY() + 6 );
        ImGui::Image( ( ImTextureID )logo_texture, ImVec2( 61, 70 ) );

        return true;
    }

    void menu::end()
    {
        ImGui::End();
    }

    bool menu::create_tab_bar()
    {
        ImGui::SetCursorPosX( ImGui::GetCursorPosX() - 1 );
        ImGui::SetCursorPosY( ImGui::GetCursorPosY() - 3 );

        ImGui::BeginChild(
            "i", ImVec2( 60, 440 ), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse );

        return true;
    }

    bool menu::create_tab(
        const char* label,
        const char* icon,
        const bool selected,
        int space,
        bool is_top_tab,
        const ImVec2& size_arg )
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if ( window->SkipItems )
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID( label );
        const ImVec2 label_size = ImGui::CalcTextSize( label, NULL, true );

        ImVec2 pos = window->DC.CursorPos;
        ImVec2 size = ImGui::CalcItemSize( size_arg, style.FramePadding.x * 2.0f, style.FramePadding.y * 2.0f );
        const ImRect bb( ImVec2( pos.x + 8, pos.y ), { pos.x + size.x + 3, pos.y + size.y } );

        ImGui::ItemSize( ImVec2( size.x, size.y ), 0 );
        if ( !ImGui::ItemAdd( bb, id ) )
            return false;

        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior( bb, id, &hovered, &held, NULL );

        ImGui::PushFont( icon_font );

        if ( !is_top_tab )
        {
            if ( selected )
            {
                window->DrawList->AddRectFilled( bb.Min, bb.Max, ImColor( 59, 66, 82 ), 9.0f );
                window->DrawList->AddText(
                    { bb.Min.x + size_arg.x / 2 - ImGui::CalcTextSize( icon ).x / 2 - 2,
                      bb.Min.y + size_arg.y / 2 - ImGui::CalcTextSize( icon ).y / 2 + 2 },
                    ImColor( 255, 255, 255 ),
                    icon );
            }
            else
            {
                window->DrawList->AddRectFilled( bb.Min, bb.Max, ImColor( 46, 52, 64 ), 9.0f );
                window->DrawList->AddText(
                    { bb.Min.x + size_arg.x / 2 - ImGui::CalcTextSize( icon ).x / 2 - 2,
                      bb.Min.y + size_arg.y / 2 - ImGui::CalcTextSize( icon ).y / 2 + 2 },
                    ImColor( 255, 255, 255 ),
                    icon );
            }
        }

        if ( is_top_tab )
        {
            if ( selected )
            {
                window->DrawList->AddRectFilled( bb.Min, bb.Max, ImColor( 59, 66, 82 ), 9.0f );
                window->DrawList->AddText(
                    { bb.Min.x + size_arg.x / 2 - ImGui::CalcTextSize( icon ).x / 2 - 2,
                      bb.Min.y + size_arg.y / 2 - ImGui::CalcTextSize( icon ).y / 2 - 5 },
                    ImColor( 255, 255, 255 ),
                    icon );

                ImGui::PushFont( small_font );
                window->DrawList->AddText(
                    { bb.Min.x + size_arg.x / 2 - ImGui::CalcTextSize( label ).x / 2 - 2,
                      bb.Min.y + size_arg.y / 2 - ImGui::CalcTextSize( label ).y / 2 + 13 },
                    ImColor( 255, 255, 255 ),
                    label );
                ImGui::PopFont();
            }
            else
            {
                window->DrawList->AddRectFilled( bb.Min, bb.Max, ImColor( 46, 52, 64 ), 9.0f );
                window->DrawList->AddText(
                    { bb.Min.x + size_arg.x / 2 - ImGui::CalcTextSize( icon ).x / 2 - 2,
                      bb.Min.y + size_arg.y / 2 - ImGui::CalcTextSize( icon ).y / 2 + 2 },
                    ImColor( 255, 255, 255 ),
                    icon );
            }
        }

        ImGui::PopFont();

        if ( !is_top_tab )
        {
            ImGui::Spacing();
            ImGui::Spacing();
        }

        return pressed;
    }

    bool menu::create_holder( const char* holder )
    {
        ImGui::SetCursorPosX( ImGui::GetCursorPosX() + 70 );
        ImGui::SetCursorPosY( ImGui::GetCursorPosY() - 50 );

        ImGuiStyle* style = &ImGui::GetStyle();
        style->ChildRounding = 4.f;

        ImVec4* colors = style->Colors;
        colors[ ImGuiCol_ChildBg ] = ImColor( 46, 52, 64 );

        ImGui::BeginChild(
            holder, ImVec2( 449, 498 ), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse );

        return true;
    }

    bool menu::create_top_holder( const char* name, bool fill )
    {
        ImGui::SetCursorPosX( ImGui::GetCursorPosX() + 120 );
        ImGui::SetCursorPosY( ImGui::GetCursorPosY() + 5 );

        ImGui::BeginChild(
            name, ImVec2( 300, 60 ), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse );

        return true;
    }

    bool menu::create_bottom_section( const char* name )
    {
        ImGui::SetCursorPosX( ImGui::GetCursorPosX() - 110 );
        ImGui::SetCursorPosY( ImGui::GetCursorPosY() + 70 );
        ImGui::BeginChild(
            name, ImVec2( 430, 415 ), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse );

        return true;
    }

    bool menu::create_section( const char* name )
    {
        ImGui::SetCursorPosX( ImGui::GetCursorPosX() + 10 );
        ImGui::SetCursorPosY( ImGui::GetCursorPosY() + 10 );
        ImGui::BeginChild(
            name, ImVec2( 430, 415 ), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse );

        return true;
    }

    bool menu::checkmark( const char* label, const char* Desc, bool* v )
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if ( window->SkipItems )
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID( label );
        const ImVec2 label_size = ImGui::CalcTextSize( label, NULL, true );

        const float square_sz = ImGui::GetFrameHeight();
        const ImVec2 pos = window->DC.CursorPos;

        int size_ = 0;

        float height = ImGui::GetFrameHeight();
        float width = height * 1.50f - 4;
        float width2 = height * 2.30f;
        float radius = height * 0.47f;

        const ImRect total_bb( pos, pos + ImVec2( square_sz + ( label_size.x + 350 ), label_size.y - 5 + 25 ) );
        ImGui::ItemSize( total_bb, style.FramePadding.y );
        if ( !ImGui::ItemAdd( total_bb, id ) )
        {
            IMGUI_TEST_ENGINE_ITEM_INFO(
                id,
                label,
                g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | ( *v ? ImGuiItemStatusFlags_Checked : 0 ) );
            return false;
        }

        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior( total_bb, id, &hovered, &held );
        if ( ImGui::IsItemClicked() )
            *v = !*v;

        const ImRect check_bb( pos, pos + ImVec2( square_sz, square_sz ) );
        ImGui::RenderNavHighlight( total_bb, id );
        // ImGui::RenderFrame(total_bb.Min, total_bb.Max, ImGui::GetColorU32((held && hovered) ? ImGuiCol_FrameBgActive :
        // hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg), true, style.FrameRounding);

        ImGui::PushFont( widget_normal );
        ImGui::PushStyleColor( ImGuiCol_Text, ImVec4( 1.f, 1.f, 1.f, 1.f ) );
        ImGui::RenderText( ImVec2( ( pos.x + width2 ) - 42, ( pos.y + height ) - 18 ), label );
        ImGui::PopStyleColor( 1 );
        ImGui::PopFont();

        float t = *v ? 1.0f : 0.0f;

        float animspeed = 0.10f;

        if ( g.LastActiveId == g.CurrentWindow->GetID( label ) )
        {
            float t_anim = ImSaturate( g.LastActiveIdTimer / animspeed );
            t = *v ? ( t_anim ) : ( 1.f - t_anim );
        }

        ImU32 col_bg;
        ImU32 col_bg2;
        if ( ImGui::IsItemHovered() )
        {
            col_bg = ImGui::GetColorU32( ImLerp( ImColor( 59, 66, 82 ), ImVec4( ImColor( 59, 66, 82 ) ), t ) );
            col_bg2 = ImGui::GetColorU32( ImLerp( ImColor( 216, 222, 233 ), ImVec4( ImColor( 216, 222, 233 ) ), t ) );
        }
        else
        {
            col_bg = ImGui::GetColorU32( ImLerp( ImColor( 59, 66, 82 ), ImVec4( ImColor( 59, 66, 82 ) ), t ) );
            col_bg2 = ImGui::GetColorU32( ImLerp( ImColor( 216, 222, 233 ), ImVec4( ImColor( 216, 222, 233 ) ), t ) );
        }

        ImGui::GetWindowDrawList()->AddRectFilled(
            ImVec2( pos.x + 390, pos.y + 8 ), ImVec2( pos.x + 420, pos.y + 25 ), col_bg, 2.f );
        ImGui::GetWindowDrawList()->AddRectFilled(
            ImVec2( pos.x + 390 + t * ( width - radius - 6 ), pos.y + 8 ),
            ImVec2( 395 + pos.x + radius + t * ( width - radius - 6 ), pos.y + radius + 14 ),
            col_bg2,
            2.f );

        ImGui::PushFont( widget_small );
        ImGui::PushStyleColor( ImGuiCol_Text, ImVec4( ImColor( 216, 222, 233 ) ) );
        ImGui::RenderText( ImVec2( ( pos.x + width2 + 6 ) - 42, ( pos.y + height ) - 5 ), Desc );
        ImGui::PopStyleColor( 1 );
        ImGui::PopFont();

        ImGui::Spacing();
        ImGui::Spacing();

        IMGUI_TEST_ENGINE_ITEM_INFO(
            id,
            label,
            g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | ( *v ? ImGuiItemStatusFlags_Checked : 0 ) );
        return pressed;
    }

    bool menu::slider( const char* label, const char* type, int* v, int v_min, int v_max )
    {
        ImGui::NewLine();

        ImGui::PushFont( widget_normal );
        menu_mgr_utils::SliderInt( label, type, v, v_min, v_max, 0, 0 );
        ImGui::PopFont();

        return true;
    }

    bool menu::slider( const char* label, const char* type, float* v, int v_min, int v_max )
    {
        ImGui::NewLine();

        ImGui::PushFont( widget_normal );
        menu_mgr_utils::SliderFloat( label, type, v, v_min, v_max, 0, 0 );
        ImGui::PopFont();

        return true;
    }

    bool
    menu::dropdown( const char* label, int* current_item, const char* const items[], int items_count, int height_in_items )
    {
        ImGui::PushFont( widget_small );

        const bool value_changed = menu_mgr_utils::ComboA(
            label, current_item, menu_mgr_utils::Items_ArrayGetter, ( void* )items, items_count, height_in_items );
        ImGui::PopFont();

        return value_changed;
    }

    bool menu::color_edit( const char* name, ImColor* col )
    {
        ImGui::Spacing();
        ImGui::Spacing();

        ImGui::PushFont( widget_normal );

        ImGui::SetCursorPosX( ImGui::GetCursorPosX() + 10 );

        ImGui::Text( name );
        ImGui::SameLine( ImGui::GetContentRegionAvail().x - 26 );

        float color[ 4 ] = { ( *col ).Value.x, ( *col ).Value.y, ( *col ).Value.z, ( *col ).Value.w };

        ImGui::PushStyleVar( ImGuiStyleVar_FrameRounding, 5.0f );
        ImGui::ColorEdit4( name, color, ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs );

        *col = ImColor{ color[ 0 ], color[ 1 ], color[ 2 ], color[ 3 ] };

        ImGui::PopStyleVar();
        ImGui::PopFont();

        ImGui::Spacing();
        ImGui::Spacing();

        return true;
    }

    void menu::initiate()
    {
        // define any styles in here
    }

    void menu::render()
    {
        if ( create_window() )
        {
            if ( create_tab_bar() )
            {
                if ( create_tab( ( "VISUALS" ), ICON_FA_EYE, tab == 0, 25, false, ImVec2( 50, 45 ) ) )
                    tab = 0;

                if ( create_tab( ( "SETTINGS" ), ICON_FA_GEAR, tab == 1, 25, false, ImVec2( 50, 45 ) ) )
                    tab = 1;

                end();
            }

            if ( tab == 0 )  // Visuals
            {
                if ( create_holder( "##blank" ) )
                {
                    if ( create_top_holder( "kys" ) )
                    {
                        if ( create_tab( ( "PLAYERS" ), ICON_FA_CUBE, tab_2 == 0, 25, true, ImVec2( 65, 55 ) ) )
                            tab_2 = 0;

                        ImGui::SameLine( 0, 0 );

                        if ( create_tab( ( "WORLD" ), ICON_FA_CUBE, tab_2 == 1, 25, true, ImVec2( 65, 55 ) ) )
                            tab_2 = 1;

                        ImGui::SameLine( 0, 0 );

                        if ( create_tab( ( "OTHER" ), ICON_FA_CUBE, tab_2 == 2, 25, true, ImVec2( 65, 55 ) ) )
                            tab_2 = 2;

                        end();
                    }

                    switch ( tab_2 )
                    {
                        case 0:
                        {
                            if ( create_bottom_section( "Players" ) )
                            {
                                checkmark(
                                    ( "Player esp" ),
                                    ( "Main toggle for all player visuals." ),
                                    &config_mgr.get< bool >( "player_esp" ) );

                                static int combo = config_mgr.get< bool >( "box_esp_dynamic" );
                                const char* items[ 2 ] = { "Static", "Dynamic" };
                                dropdown( "Box esp type", &combo, items, 2, 0 );
                                config_mgr.get< bool >( "box_esp_dynamic" ) = combo;

                                ImGui::NewLine();

                                checkmark(
                                    ( "Name esp" ),
                                    ( "Draws the player's name above them." ),
                                    &config_mgr.get< bool >( "name_esp" ) );
                                checkmark(
                                    ( "Autoscale names" ),
                                    ( "Player names will scale based on camera distance." ),
                                    &config_mgr.get< bool >( "autoscale_names" ) );

                                /*

                                   heres how to use the other funcs for u

                                   static int combo = 0;
                                       const char* combo_items[6] = { "None", "ihateu", "cysucks", "nicksicks",
                                   "celestialsucks", "idkanymore" }; dropdown("Type", &combo, combo_items, 6, 0);

                                   static int val;
                                   slider("Line Width", "studs", &val, 0, 1000);

                                   static float color[4] = { 1.f, 1.f, 1.f, 1.f };
                                   color_edit("bye", color);
                                */

                                end();
                            }
                            break;
                        }
                        case 1:
                        {
                            break;
                        }
                        case 2:
                        {
                            if ( create_bottom_section( "Others" ) )
                            {
                                color_edit( "Ally color", &config_mgr.get< ImColor >( "ally_color" ) );
                                color_edit( "Enemy color", &config_mgr.get< ImColor >( "enemy_color" ) );
                                checkmark(
                                    "Use team color",
                                    "Uses in game team colors for player esp.",
                                    &config_mgr.get< bool >( "use_team_color" ) );
                                color_edit( "FOV color", &config_mgr.get< ImColor >( "fov_color" ) );
                                checkmark(
                                    "Show FOV",
                                    "Renders a circle in middlg of screen visualizing FOV",
                                    &config_mgr.get< bool >( "fov_circle" ) );
                                checkmark(
                                    "Team check",
                                    "Renders players if they are on an opposing team only.",
                                    &config_mgr.get< bool >( "team_check" ) );
                                checkmark(
                                    "Distance check",
                                    "Only renders visuals within render distance of the camera.",
                                    &config_mgr.get< bool >( "distance_check" ) );
                                slider( "Render distance", "studs", &config_mgr.get< int >( "render_distance" ), 25, 1000 );
                                slider( "FOV size", "", &config_mgr.get< float >( "fov_size" ), 100, 1000 );
                                /*

                                   heres how to use the other funcs for u

                                   static int combo = 0;
                                       const char* combo_items[6] = { "None", "ihateu", "cysucks", "nicksicks",
                                   "celestialsucks", "idkanymore" }; dropdown("Type", &combo, combo_items, 6, 0);

                                   static int val;
                                   slider("Line Width", "studs", &val, 0, 1000);

                                   static float color[4] = { 1.f, 1.f, 1.f, 1.f };
                                   color_edit("bye", color);
                                */

                                end();
                            }
                            break;
                        }
                    }

                    end();
                }
            }
            else if ( tab == 1 )
            {
                if ( create_holder( "##blank" ) )
                {
                    if ( create_section( "settings" ) )
                    {
                        checkmark(
                            ( "Explorer toggle" ),
                            ( "Shows a simple explorer that allows you to inspect Roblox's data model." ),
                            &config_mgr.get< bool >( "explorer" ) );

                        end();
                    }
                }
            }

            end();
        }

        if ( config_mgr.get< bool >( "explorer" ) )
            render_explorer();
        ;
    }

    void menu::imgui_checkbox( const char* const name, std::string_view feature )
    {
        bool value = config_mgr.get< bool >( feature );

        ImGui::Checkbox( name, &value );

        config_mgr.set< bool >( feature, value );
    }

    void menu::render_explorer()
    {
        const auto explorer_width = 400;
        const auto properties_height = 400;

        const auto viewport = ImGui::GetMainViewport()->Size;

        ImGui::SetNextWindowSize( ImVec2( explorer_width, viewport.y - properties_height ) );
        ImGui::SetNextWindowPos( ImVec2( viewport.x - explorer_width, 0 ) );

        ImGui::Begin(
            "explorer", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse );

        std::string s;
        ImGui::InputText( "Search", &s );

        ImGui::NewLine();

        render_explorer_nodes( *process_hook_mgr.data_model, true );

        ImGui::End();

        // draw the properties tab (bottom right)
        ImGui::SetNextWindowSize( ImVec2( explorer_width, properties_height ) );
        ImGui::SetNextWindowPos( ImVec2( viewport.x - explorer_width, viewport.y - properties_height ) );
        ImGui::Begin(
            "properties", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse );

        render_explorer_properties();

        ImGui::End();
    }

    void menu::render_explorer_nodes( rbx::instance instance, bool require_children )
    {
        for ( const auto& child : instance.get_children() )
        {
            const auto size = child.get_children().size();

            ImGui::PushID( "##unique_id" );

            if ( !size )
            {
                if ( require_children )
                    continue;

                ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
                ImGui::TreeNodeEx( child.name().c_str(), flags );

                ImGui::PopID();
            }

            else if ( ImGui::TreeNode( child.name().c_str() ) )
            {
                ImGui::PopID();

                render_explorer_nodes( child );
                ImGui::TreePop();
            }

            if ( ImGui::IsItemClicked() )
                selected = child;
        }
    }

    void menu::render_explorer_properties()
    {
        if ( selected.get_address() )
        {
            ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 2, 2 ) );
            if ( ImGui::BeginTable( "split", 2, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable ) )
            {
                // name
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex( 0 );
                ImGui::AlignTextToFramePadding();

                ImGui::Text( "Name" );

                ImGui::TableSetColumnIndex( 1 );
                ImGui::SetNextItemWidth( -FLT_MIN );

                ImGui::Text( selected.name().c_str() );
                ImGui::NextColumn();

                // classname
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex( 0 );
                ImGui::AlignTextToFramePadding();

                ImGui::Text( "ClassName" );

                ImGui::TableSetColumnIndex( 1 );
                ImGui::SetNextItemWidth( -FLT_MIN );

                ImGui::Text( selected.descriptor().name().c_str() );
                ImGui::NextColumn();

                // parent
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex( 0 );
                ImGui::AlignTextToFramePadding();

                ImGui::Text( "Parent" );

                ImGui::TableSetColumnIndex( 1 );
                ImGui::SetNextItemWidth( -FLT_MIN );

                ImGui::Text( selected.parent().name().c_str() );
                ImGui::NextColumn();
                ImGui::EndTable();
            }
            ImGui::PopStyleVar();
        }
    }

    menu menu_mgr;

}  // namespace nord