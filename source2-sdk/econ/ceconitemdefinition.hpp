#pragma once
#include <cstring>
#include <cstddef>

#include "../../virtual.hpp"

#include "../types/utlvector.hpp"

class CEconItemDefinition {
   public:
    bool IsWeapon();
    bool IsKnife(bool excludeDefault);
    bool IsGlove(bool excludeDefault);

    auto GetModelName() {
        return *reinterpret_cast<const char**>((uintptr_t)(this) + 0x148);
    }

    auto GetStickersSupportedCount() {
        return *reinterpret_cast<int*>((uintptr_t)(this) + 0x168);
    }

    auto GetSimpleWeaponName() {
        if (!m_inventory_image) return "";
        const char* slash = std::strrchr(m_inventory_image, '/');
        return slash ? slash + 1 : m_inventory_image;
    }

    auto GetLoadoutSlot() {
        return *reinterpret_cast<int*>((uintptr_t)(this) + 0x338);
    }

 char pad0[0x8];
 void* m_pKVItem;
 uint16_t m_nDefIndex;
 char pad1[0x1E];
 bool m_bEnabled;
 char pad2[0xF];
 uint8_t m_unMinItemLevel,m_unMaxItemLevel,m_nItemRarity,m_nItemQuality,m_nForcedItemQuality,m_nDefaultDropItemQuality,m_nDefaultDropQuantity;
 char pad3[0x19];
 uint8_t m_nPopularitySeed;
 char pad4[7];
 void* m_pPortraitsKV;
 const char* m_pszItemBaseName;
 bool m_bProperName;
 char pad5[7];
 const char* m_pszItemTypeName;
 uint32_t m_unItemTypeID;
 char pad6[4];
 const char* m_pszItemDesc;
 char pad7[0x10];
 const char* m_inventory_image;

};
static_assert(offsetof(CEconItemDefinition, m_pszItemBaseName) == 0x70);
