#include <stb/stb.hh>
#include "cgcclientsharedobjectcache.hpp"

#include "../../memory/memory.hpp"

CGCClientSharedObjectTypeCache* CGCClientSharedObjectCache::CreateBaseTypeCache(
    int nClassID) {
    
    static auto fn = CModule(CLIENT_DLL).FindPattern(SDK_SIG("40 ? 48 83 EC ? 4C 8B 49 ? 44 8B D2")).Get<decltype(memory::fnCreateBaseTypeCache)>();
    return fn ? fn(this, nClassID) : nullptr;
}
