#pragma once

#include "../../virtual.hpp"

class CHeapMemAlloc {
   public:
    template <typename T = void*>
    auto Alloc(size_t nSize) {
        return CALL_VIRTUAL(T, 1, this, nSize);
    }

    template <typename T = void*>
    auto Realloc(T pMem, size_t nSize) {
        return CALL_VIRTUAL(T, 2, this, pMem, nSize);
    }

    auto Free(void* pMem) { return CALL_VIRTUAL(void, 3, this, pMem); }
} inline* g_pHeapMemAlloc;
