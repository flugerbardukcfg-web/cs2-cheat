#include <stb/stb.hh>
#include "../../../api/module/module.hpp"
#include "../../../utils/native_rtti.hpp"
#pragma once

#include "../../virtual.hpp"

class CCSPlayerInventory;

class CCSInventoryManager {
   public:
    static CCSInventoryManager* GetInstance();

    auto EquipItemInLoadout(int iTeam, int iSlot, uint64_t iItemID) {
        auto* inventory = GetLocalInventory();
        if (!inventory || iTeam < 0 || iTeam >= 4 || iSlot < 0 || iSlot >= 58) return false;
        using fn_t = void(__fastcall*)(void*,uint16_t,uint16_t,uint64_t,uint16_t,bool);
        static auto fn = CModule(CLIENT_DLL).FindPattern(SDK_SIG("48 8B C4 56 57 41 56 41 57 48 83 EC 48 0F B7 F2 4D 8B F9 41 0F B7 F8 4C 8B F1 83 FE 04 0F 83 ? ? ? ? 83 FF 3A 0F 83 ? ? ? ? 48 89 58 08 44 8B")).Get<fn_t>();
        if (!fn) return false;
        fn(inventory,static_cast<uint16_t>(iTeam),static_cast<uint16_t>(iSlot),iItemID,0,true);
        return true;
    }

    CCSPlayerInventory* GetLocalInventory() {
        static auto offset = NativeRtti::InventoryOffsetFromUseTool(CModule(CLIENT_DLL).FindPattern(SDK_SIG("48 85 D2 0F 84 ? ? ? ? 53 41 57 48 83 EC 28 49 8B D8 4C 8B F9 48 85 DB 0F 84")).Get<const uint8_t*>());
        return offset ? *reinterpret_cast<CCSPlayerInventory**>(reinterpret_cast<uintptr_t>(this) + offset) : nullptr;
    }
};
