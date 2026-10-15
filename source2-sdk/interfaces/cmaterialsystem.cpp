#include "cmaterialsystem.hpp"

void KeyValues3::del(void* pMem)
{
	if (!memory::fnDelete) return;
	return memory::fnDelete(pMem, 0, nullptr, 0);
}
KeyValues3* KeyValues3::set_type(kv_basic_type a1, kv_basic_type a2)
{
	if (!fnSetType) return nullptr;
	return fnSetType(this, 6, 1);
}
bool KeyValues3::load_from_buffer(CUtlBuffer* buffer, const char* name)
{
	KV3ID_t type{ "generic", 0x469806E97412167C, 0xE73790B53EE6F2AF };
	return fnLoadKV3 && buffer && fnLoadKV3(this, nullptr, buffer, &type, "", 0);
}   


KeyValues3* KeyValues3::SetType(uint8_t a1, uint8_t a2)
{
	return fnSetType(this, 6, 1);
}

bool KeyValues3::LoadKV3FromKV3OrKV1(CUtlString* ErrorStr, CUtlBuffer* Text, const char** szStrangeConvertType, const char* a4)
{
	static auto LoadKV3FromKV3OrKV1Fn = reinterpret_cast<bool(__fastcall*)(KeyValues3*, CUtlString*, CUtlBuffer*, const char**, const char*)>
		(GetProcAddress(GetModuleHandleA(("tier0.dll")), ("?LoadKV3FromKV3OrKV1@@YA_NPEAVKeyValues3@@PEAVCUtlString@@PEAVCUtlBuffer@@AEBUKV3ID_t@@PEBD@Z")));

	return LoadKV3FromKV3OrKV1Fn(this, ErrorStr, Text, szStrangeConvertType, a4);
}

KeyValues3* KeyValues3::FreeAlloc(uint32_t nSize)
{
	static auto FreeAllocationFn = reinterpret_cast<KeyValues3 * (__fastcall*)(KeyValues3*, uint32_t)>(pFnFreeAlloc);
	return FreeAllocationFn(this, nSize);
}
bool KeyValues3::LoadFromText(const char* text) {
 using fn_t = bool(__fastcall*)(KeyValues3*, void*, const char*, const KV3ID_t&, const char*, unsigned);
 static auto fn = reinterpret_cast<fn_t>(GetProcAddress(GetModuleHandleA("tier0.dll"), "?LoadKV3@@YA_NPEAVKeyValues3@@PEAVCUtlString@@PEBDAEBUKV3ID_t@@2I@Z"));
 const KV3ID_t format{"generic",0x469806E97412167C,0xE73790B53EE6F2AF};
 return fn && text && fn(this,nullptr,text,format,"",0);
}
