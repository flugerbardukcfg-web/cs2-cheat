#include "../../sdk/interfaces/interfaces.hpp"
#include "world.hpp"
#include <unordered_map>
#include <cstring>

void world::SkyBoxChanger()
{
    struct Saved { Color_t tint; Color_t lighting; float brightness; };
    static std::unordered_map<uint32_t, Saved> saved;
    auto* system = CGameEntitySystem::GetInstance();
    if (!system) return;
    for (auto it = saved.begin(); it != saved.end();) {
        CHandle handle; handle.m_Index = it->first;
        if (!handle.Get()) it = saved.erase(it); else ++it;
    }
    for (int i = 1, highest = system->GetHighestEntityIndex(); i <= highest; ++i) {
        auto* entity = system->GetBaseEntity(i);
        auto* info = entity ? entity->Schema_DynamicBinding() : nullptr;
        if (!info || !info->GetName() || std::strcmp(info->GetName(), "C_EnvSky") != 0) continue;
        auto* sky = static_cast<C_EnvSky*>(entity);
        const auto handle = sky->GetRefEHandle();
        const auto key = handle.m_Index;
        if (world::skybox || world::nightmode) {
            saved.try_emplace(key, Saved{sky->m_vTintColor(), sky->m_vTintColorLightingOnly(), sky->m_flBrightnessScale()});
            sky->m_vTintColor() = Color_t(0, 0, 0, 255);
            sky->m_vTintColorLightingOnly() = Color_t(0, 0, 0, 255);
            sky->m_flBrightnessScale() = 0.f;
        } else if (auto it = saved.find(key); it != saved.end()) {
            sky->m_vTintColor() = it->second.tint;
            sky->m_vTintColorLightingOnly() = it->second.lighting;
            sky->m_flBrightnessScale() = it->second.brightness;
            saved.erase(it);
        }
    }
}
