#pragma once

#include "../../virtual.hpp"

class ConVar {
   public:
    const char* m_name;

    template <typename T>
    T GetValue() {
         
         
        return *reinterpret_cast<T*>((uintptr_t)(this) + 0x58);
    }
};

class CCvar {
   public:
    static CCvar& Get() {
        static CCvar inst;
        return inst;
    }
    struct List {
        struct Entry { ConVar* value; uint16_t generation, next; uint32_t links; };
        uint16_t reserved, allocation; uint32_t reserved2;
        Entry* entries;
        uint16_t head, tail, free, count;
        int Capacity() const { return allocation & 0x7fff; }
        bool Valid(int i) const { return entries && i >= 0 && i < Capacity(); }
        uint64_t First() const { return Valid(head) ? head : 0xffffffffULL; }
        uint64_t Next(uint64_t i) const { return entries && i < static_cast<uint64_t>(Capacity()) && Valid(entries[i].next) ? entries[i].next : 0xffffffffULL; }
    };
    void* GetFirstCvarIterator(uint64_t& i) { i = GetList().First(); return &i; }
    void* GetNextCvarIterator(uint64_t& i) { i = GetList().Next(i); return &i; }
    ConVar* FindVarByIndex(uint64_t i) { auto& list = GetList(); return list.entries && i < static_cast<uint64_t>(list.Capacity()) ? list.entries[i].value : nullptr; }
    ConVar* FindVarByName(const char* name) {
        if (!name) return nullptr;
        auto& list = GetList();
        for (uint64_t i = list.First(), visited = 0; i != 0xffffffffULL && visited < static_cast<uint64_t>(list.Capacity()); i = list.Next(i), ++visited) {
            auto* var = FindVarByIndex(i);
            if (var && var->m_name && !strcmp(var->m_name, name)) return var;
        }
        return nullptr;
    }
private:
    List& GetList() { return *reinterpret_cast<List*>(reinterpret_cast<uintptr_t>(this) + 0x48); }
};
static_assert(sizeof(CCvar::List::Entry) == 0x10);
