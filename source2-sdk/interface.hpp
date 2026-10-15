#pragma once

typedef void *(*InstantiateInterfaceFn)();

 
struct InterfaceReg {
    InstantiateInterfaceFn m_CreateFn;
    const char *m_pName;
    InterfaceReg *m_pNext;   
};
