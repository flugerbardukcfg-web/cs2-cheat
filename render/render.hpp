#pragma once
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
namespace render {
    void Initialize();
    void NewFrame();
    void text_outline(ImDrawList* draw_list, const ImVec2& pos, ImU32 col,
                      const char* text);
    void corner_outline(ImDrawList* draw, float x1, float y1, float x2,
                               float y2, ImColor color, ImColor color_outline,
                               float th = 1.f, float rounding = 0.f);
}   
