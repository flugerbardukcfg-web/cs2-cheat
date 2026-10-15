#pragma once
#include <stb/stb.hh>

#include "../../virtual.hpp"
#include "../../../api/module/module.hpp"

class CGameEntitySystem;

class CGameResourceService {
   public:
    CGameEntitySystem* GetGameEntitySystem() {
        static auto slot = CModule(CLIENT_DLL).FindPattern(SDK_SIG("48 8B 0D ? ? ? ? 48 8B D3 E8 ? ? ? ? FF C5")).ToAbsolute(3, 0).Get<CGameEntitySystem**>();
        return slot ? *slot : nullptr;
    }
};
