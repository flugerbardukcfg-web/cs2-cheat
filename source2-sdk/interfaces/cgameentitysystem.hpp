#pragma once

#include "../../memory/memory.hpp"
#include "../../virtual.hpp"

#include "../entity/ccsplayercontroller.hpp"
#include "../entity/c_baseviewmodel.hpp"
#include "../entity/c_chicken.hpp"

class C_BaseEntity;

class CGameEntitySystem {
   public:
    static CGameEntitySystem* GetInstance();
    static CCSPlayerController* GetLocalPlayerController();

    template <typename T = C_BaseEntity>
    T* GetByIndex(CHandle nIndex)
    {
        return reinterpret_cast<T*>(this->GetEntityByIndex(nIndex.GetEntryIndex()));
    }

    static CGameEntitySystem& Get() {
        static CGameEntitySystem inst;
        return inst;
    }
    template <typename T = C_BaseEntity>
    T* GetBaseEntity(int index) {
        return reinterpret_cast<T*>(GetEntityByIndex(index));
    }

    void* GetEntityByIndex(int nIndex)
    {
         
         
        if (nIndex < 0 || nIndex >= 0x7fff) return nullptr;
        auto* chunk = *reinterpret_cast<uint8_t**>(reinterpret_cast<uintptr_t>(this) + 0x10 + 8 * (nIndex >> 9));
        if (!chunk) return nullptr;
        auto* identity = chunk + 0x70 * (nIndex & 0x1ff);
        if ((*reinterpret_cast<uint32_t*>(identity + 0x10) & 0x7fff) != static_cast<uint32_t>(nIndex)) return nullptr;
        return *reinterpret_cast<void**>(identity);
    }

    int GetHighestEntityIndex() {
        auto** chunks = reinterpret_cast<uint8_t**>(reinterpret_cast<uintptr_t>(this) + 0x10);
        for (int chunk = 63; chunk >= 0; --chunk) {
            if (!chunks[chunk]) continue;
            for (int slot = 511; slot >= 0; --slot) {
                int index = chunk * 512 + slot;
                if (index >= 0x7fff) continue;
                auto* identity = chunks[chunk] + slot * 0x70;
                if ((*reinterpret_cast<uint32_t*>(identity + 0x10) & 0x7fff) == static_cast<uint32_t>(index) && *reinterpret_cast<void**>(identity)) return index;
            }
        }
        return -1;
    }
     
     
     

     
     
     
};
