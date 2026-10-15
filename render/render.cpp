#include <thread>

#include "../../utils/utils.hpp"

#include "../menu/menu.hpp"
#include "../esp/esp.hpp"

#include "render.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_freetype.h>

#include "font.h"
#include "../esp/esp.hpp"

void render::Initialize( )
{
	ImGuiIO& IO = ImGui::GetIO( );

	ImVector<ImWchar> ranges;
	ImFontGlyphRangesBuilder builder;

	 
	ImFontConfig font_config;
	 
	 
								  
								  
	visuals::pixel_font = IO.Fonts->AddFontFromMemoryTTF((void*) esp, sizeof(esp), 8.f);
	 
	 
	font_config.OversampleH = 3;
	font_config.OversampleV = 5;
	font_config.PixelSnapH = true;
	 

	visuals::menu_font = IO.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 12, &font_config, IO.Fonts->GetGlyphRangesCyrillic( ));
	 
}
void render::NewFrame( )
{
	visuals::Render( );
	menu::Render( );
}
void render::text_outline(ImDrawList* draw_list, const ImVec2& pos, ImU32 col,
						  const char* text)
{
	ImGui::PushFont(visuals::menu_font);
	 
	 
	 
	 
	 
	 
	 
	 

	 
	 
	 
	 
	 
	 
	 
	 
	draw_list->AddText(ImVec2(pos.x, pos.y), col, text);
	ImGui::PopFont( );
}

template <class T>
inline void line(ImDrawList* draw, T x1, T y1, T x2, T y2, ImColor color,
				 float thickness = 1.f)
{
	draw->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), color, thickness);
}
template <class T>
inline void outline(ImDrawList* draw, T x1, T y1, T x2, T y2, ImColor color,
					float rounding = 0.f)
{
	draw->AddRectFilled(ImVec2(x1 - 1, y1 - 1), ImVec2(x2 + 1, y2 + 1), color,
						rounding);
}
template <class T>
inline void glow(ImDrawList* draw, T x1, T y1, T x2, T y2, ImColor color,
				 float rounding = 0.f)
{}

void render::corner_outline(ImDrawList* draw, float x1, float y1, float x2,
							float y2,
							ImColor color, ImColor color_outline, float th,
							float rounding)
{
	int w = x2 - x1;
	int h = y2 - y1;

	int iw = w / 4;
	int ih = h / 4;


	 
	 

	draw->AddRectFilled(ImVec2(x1, y1), ImVec2(x2, y2), color, rounding);


	 
	outline(draw, x1, y1, x1 + iw, y1 + 1, color_outline, rounding);
	outline(draw, x1 + w - iw, y1, x1 + w, y1 + 1, color_outline, rounding);
	outline(draw, x1, y1, x1 + 1, y1 + ih, color_outline, rounding);
	outline(draw, x1 + w - 1, y1, x1 + w, y1 + ih, color_outline, rounding);
	 
	outline(draw, x1, y1 + h, x1 + iw, y1 + h + 1, color_outline, rounding);
	outline(draw, x1 + w - iw, y1 + h, x1 + w, y1 + h + 1, color_outline, rounding);
	outline(draw, x1, y1 + h - ih, x1 + 1, y1 + h, color_outline, rounding);
	outline(draw, x1 + w - 1, y1 + h - ih, x1 + w, y1 + h, color_outline, rounding);

}
