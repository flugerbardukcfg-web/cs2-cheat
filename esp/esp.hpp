#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "../../sdk/source2-sdk/types/chandle.hpp"
#include <d3d11.h>

class CEntityInstance;

namespace visuals {
    void Render();
    inline ImFont* pixel_font;
    inline ImFont* menu_font;
    inline ImFont* esp_font;
    void CalculateBoundingBoxes();
    void CacheCurrentEntities();
    void OnAddEntity(CEntityInstance* pInst, CHandle handle);
    void OnRemoveEntity(CEntityInstance* pInst, CHandle handle);

     
     
     
    inline float fWeaponMaxDistance;
    inline bool esp_enable;
    inline bool esp_box;
    inline bool esp_name;
    inline bool esp_health;
    inline bool nightmode;
    inline bool esp_glow;

    inline ID3D11ShaderResourceView* BG = nullptr;
    inline ID3D11ShaderResourceView* legitBot = nullptr;
    inline ID3D11ShaderResourceView* _visuals = nullptr;
    inline ID3D11ShaderResourceView* _misc = nullptr;
    inline ID3D11ShaderResourceView* _skins = nullptr;
    inline ID3D11ShaderResourceView* _configs = nullptr;
    inline ID3D11ShaderResourceView* check_box = nullptr;

    inline ImDrawList* g_pBackgroundDrawList = nullptr;
}   
