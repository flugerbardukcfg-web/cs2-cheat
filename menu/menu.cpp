#include <algorithm>
#include <vector>
#define IMGUI_DEFINE_MATH_OPERATORS
#include "menu.hpp"

#include "../../sdk/interfaces/interfaces.hpp"
#include "../../utils/utils.hpp"
#include "../esp/esp.hpp"
#include "../chams/chams.hpp"
 

#include <imgui/imgui.h>

#include "../world/world.hpp"
#include "../external_aim/aim.h"
#include "../skins/knife.h"
#include "../../utils/external/config.hpp"

static bool g_bMenuIsOpen;

int tab = 0;

int prefer = 3;


void CustomSliderInt(const char* label, int* v, float v_min, float v_max, bool checker_bool, const char* format)
{
    auto posSlider = ImGui::GetCursorScreenPos();
    if (checker_bool)
    {
        ImGui::SliderInt(label, v, v_min, v_max, format);
    }
    else
    {
        ImGui::BeginDisabled(true);
        ImGui::SliderInt(label, v, v_min, v_max, format);
        ImGui::EndDisabled();
        ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(posSlider.x + 16, posSlider.y), ImVec2(posSlider.x + 180, posSlider.y + 37), ImColor(22, 22, 22, 155));
    }
}

void CustomSliderFloat(const char* label, float* v, float v_min, float v_max, bool checker_bool)
{
    auto posSlider = ImGui::GetCursorScreenPos();
    if (checker_bool)
    {
        ImGui::SliderFloat(label, v, v_min, v_max, "%.0f");
    }
    else
    {
        ImGui::BeginDisabled(true);
        ImGui::SliderFloat(label, v, v_min, v_max, "");
        ImGui::EndDisabled();
        ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(posSlider.x + 16, posSlider.y), ImVec2(posSlider.x + 180, posSlider.y + 37), ImColor(22, 22, 22, 155));
    }
}

void custom_cursor(ImVec2 position, float rotation_angle_degrees, ImColor color) {
    float arrow_base_width = 8;
    float arrow_height = 9;
    float arrow_middle_width = 7;
    float outline_thickness = 2.0f;


    ImVec2 verts[4];
    verts[0] = position;
    verts[1] = ImVec2(position.x - arrow_base_width / 2, position.y + arrow_height);
    verts[2] = ImVec2(position.x, position.y + arrow_middle_width);
    verts[3] = ImVec2(position.x + arrow_base_width / 2, position.y + arrow_height);


    ImVec2 center = ImVec2(position.x, position.y + arrow_height / 2);

    float angle_radians = rotation_angle_degrees * (3.14159f / 180.0f);

    auto rotate_vertex = [&](ImVec2 point, ImVec2 center, float angle) -> ImVec2 {
        float translated_x = point.x - center.x;
        float translated_y = point.y - center.y;

        float rotated_x = translated_x * cos(angle) - translated_y * sin(angle);
        float rotated_y = translated_x * sin(angle) + translated_y * cos(angle);

        return ImVec2(rotated_x + center.x, rotated_y + center.y);
        };

    verts[0] = rotate_vertex(verts[0], center, angle_radians);
    verts[1] = rotate_vertex(verts[1], center, angle_radians);
    verts[2] = rotate_vertex(verts[2], center, angle_radians);
    verts[3] = rotate_vertex(verts[3], center, angle_radians);

    ImDrawList* draw_list = ImGui::GetForegroundDrawList();

    draw_list->AddPolyline(verts, 4, color, true, outline_thickness);

    draw_list->AddConvexPolyFilled(verts, 4, color);
}

HCURSOR CreateNullCursor() {
    BYTE andMask[1] = { 0xFF };   
    BYTE xorMask[1] = { 0x00 };   
    return CreateCursor(NULL, 0, 0, 1, 1, andMask, xorMask);
}

void menu::Render()
{
	ImGuiIO& IO = ImGui::GetIO();
	IO.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

	if (ImGui::IsKeyPressed(ImGuiKey_Insert, false)) Toggle(!IsOpen());
	if (!IsOpen()) return;

     
     

	IO.ConfigFlags = ImGuiConfigFlags_None;

	ImGui::PushFont(visuals::menu_font);

    ImGui::SetNextWindowSize(ImVec2(477, 418));
    ImGui::Begin("RAZECLUB", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
    {
        auto draw = ImGui::GetWindowDrawList();
        auto pos = ImGui::GetWindowPos();

        draw->AddImage(visuals::BG, ImVec2(pos.x + 4, pos.y + 2), ImVec2(pos.x + 472, pos.y + 417));

        switch (tab)
        {
        case 0:
            draw->AddImage(visuals::legitBot, ImVec2(pos.x + 14 + 4, pos.y + 12 + 2), ImVec2(pos.x + 58 + 4, pos.y + 53.2 + 2));
            break;
        case 1:
            draw->AddImage(visuals::_visuals, ImVec2(pos.x + 8 + 4, pos.y + 52 + 2), ImVec2(pos.x + 58 + 4, pos.y + 99 + 2));
            break;
        case 2:
            draw->AddImage(visuals::_misc, ImVec2(pos.x + 11 + 4, pos.y + 98 + 2), ImVec2(pos.x + 55 + 4, pos.y + 140 + 2));
            break;
        case 3:
            draw->AddImage(visuals::_skins, ImVec2(pos.x + 11 + 4, pos.y + 139 + 2), ImVec2(pos.x + 55 + 4, pos.y + 183 + 2));
            break;
        case 4:
            draw->AddImage(visuals::_configs, ImVec2(pos.x + 12 + 4, pos.y + 184 + 2), ImVec2(pos.x + 54 + 4, pos.y + 226 + 2));
            break;
        }

        ImGui::SetCursorPos(ImVec2(4, 4));
        ImGui::BeginChild("LeftIcons", ImVec2(57, 417));
        {
            if (ImGui::InvisibleButton("Legitbot", ImVec2(55, 45)))
                tab = 0;
            if (ImGui::InvisibleButton("Visuals", ImVec2(55, 35)))
                tab = 1;
            if (ImGui::InvisibleButton("Misc", ImVec2(55, 45)))
                tab = 2;
            if (ImGui::InvisibleButton("Skins", ImVec2(55, 35)))
                tab = 3;
            if (ImGui::InvisibleButton("Configs", ImVec2(55, 45)))
                tab = 4;
        }
        ImGui::EndChild();

        ImGui::SetCursorPos(ImVec2(77, 25));
        ImGui::BeginChild("LeftPanel", ImVec2(185, 375));
        {
            switch (tab)
            {
            case 0:
                ImGui::Checkbox("Enable aimbot", &aimConf.state);

                ImGui::Combo("Hitbox", &aimConf.boneSelect, "Head\0Neck\0Chest\0Crotch\0", 6);

                ImGui::Checkbox("Aimlock", &aimConf.alaimlock);
                ImGui::Checkbox("Visible check", &aimConf.checkSpotted);
                ImGui::Checkbox("Player lock", &aimConf.playerLock);
                ImGui::Checkbox("Engine Prediction [Alpha]", &aimConf.predictionTest);

                CustomSliderFloat("Aimbot FOV", &aimConf.fov, 0, 15, aimConf.state);

                CustomSliderFloat("Aimbot smooth", &aimConf.smoothing, 1.0, 30, aimConf.state);

                

                 

                ImGui::Checkbox("Trigger bot", &aimConf.trigger);
                ImGui::Checkbox("Trigger bot on key [ALT]", &aimConf.isHotTrigger);

                CustomSliderInt("Trigger reaction", &aimConf.trigger_reaction, 0, 400, aimConf.trigger, "%d ms");

                ImGui::Checkbox("Recoil sperm system", &aimConf.rcs);

                config::save(0);
                break;

            case 1:
                ImGui::Checkbox("Enable", &visuals::esp_enable);
                ImGui::Checkbox("Esp box", &visuals::esp_box);
                ImGui::Checkbox("Esp name", &visuals::esp_name);
                ImGui::Checkbox("Esp health", &visuals::esp_health);
                ImGui::Checkbox("Esp glow", &visuals::esp_glow);
                ImGui::Checkbox("Esp bomb", &bombcheck);

                ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

                ImGui::Checkbox("Chams visible", &chams::player);
                CustomSliderInt("Visible chams intensity", &chams::visible_value, 0, 255, chams::player, "");

                ImGui::Checkbox("Chams invisible", &chams::invisible);
                CustomSliderInt("Invisible chams intensity", &chams::invisible_value, 0, 255, chams::invisible, "");

                ImGui::Checkbox("Model glow", &other::glow);
                CustomSliderInt("Model glow intensity", &other::intesivity, 0, 255, other::glow, "");

                 

                 
                break;
            case 2:
                ImGui::ShadowText("SEX TEXT 1", ImColor(255, 255, 255), ImColor(255, 255, 255, 145), 25, ImVec2(pos.x + 89, pos.y + 31));
                break;
            case 3:
                ImGui::Checkbox("Knife model", &knife_changer::enable);
                ImGui::Combo("##combo", &knife_changer::selected, "-\0M9 Bayonet\0Classic\0Bayonet\0Flip\0Gut\0Karambit\0Tactical\0Falchion\0Bowie\0Butterfly\0Push\0Cord\0Canis\0Ursus\0Navaja\0Outdoor\0Stiletto\0Talon\0Skeleton\0Kukri", 7);
                break;
            }
        }
        ImGui::EndChild();

        ImGui::SetCursorPos(ImVec2(273, 25));
        ImGui::BeginChild("RightPanel", ImVec2(185, 375));
        {
            switch (tab)
            {
            case 0:
                break;
            case 1:
                ImGui::Checkbox("Nightmode", &world::nightmode);
                ImGui::Checkbox("Remove legs", &legscheck);
                ImGui::Checkbox("Remove flash", &flashcheck);
                break;
            case 2:
                ImGui::ShadowText("SEX TEXT 2", ImColor(255, 255, 255), ImColor(255, 255, 255, 145), 25, ImVec2(pos.x + 285, pos.y + 31));
                break;
            }
        }
        ImGui::EndChild();
    }
    ImGui::End();

	ImGui::PopFont();
}

void menu::Toggle(bool bState)
{
    static bool relativeMouseMode = false;
    void* window = memory::fnSDL_GetGrabbedWindow ? memory::fnSDL_GetGrabbedWindow() : nullptr;
    if (bState && !g_bMenuIsOpen && window && memory::fnSDL_GetRelativeMouseMode)
        relativeMouseMode = memory::fnSDL_GetRelativeMouseMode(window);
    g_bMenuIsOpen = bState;
    if (window && memory::fnSDL_SetRelativeMouseMode)
        memory::fnSDL_SetRelativeMouseMode(window, bState ? false : relativeMouseMode);
}

bool menu::IsOpen()
{
	return g_bMenuIsOpen;
}
