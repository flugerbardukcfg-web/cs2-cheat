#include "../../../utils/native_rtti.hpp"
#include "ceconitem.hpp"

#include "../../interfaces/interfaces.hpp"
#include "../../memory/memory.hpp"

void CEconItem::SetDynamicAttributeValue(int index, void* value) {
    auto* system = interfaces::pClient ? interfaces::pClient->GetEconItemSystem() : nullptr;
    CEconItemSchema* pItemSchema = system ? system->GetEconItemSchema() : nullptr;
    if (!pItemSchema) return;

    void* pAttributeDefinitionInterface =
        pItemSchema->GetAttributeDefinitionInterface(index);
    if (!pAttributeDefinitionInterface) return;

    static auto floating = NativeRtti::SetterFromAttributeType(GetModuleHandleA(CLIENT_DLL),".?AVCSchemaAttributeType_Float@@");
    static auto integer = NativeRtti::SetterFromAttributeType(GetModuleHandleA(CLIENT_DLL),".?AVCSchemaAttributeType_Uint32@@");
    auto fn = index == 6 || index == 7 || index == 8 ? floating : integer;
    if (fn && value) fn(this,pAttributeDefinitionInterface,value);
}

void CEconItem::SetDynamicAttributeValueString(int index, const char* value) {
     
}

CEconItem* CEconItem::CreateInstance() {
    if (!memory::fnCreateSharedObjectSubclassEconItem) return nullptr;
    return memory::fnCreateSharedObjectSubclassEconItem();
}
