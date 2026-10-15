#pragma once

#include "../../api/module/module.hpp"

#include "../source2-sdk/cstrike15/ccsplayerinventory.hpp"
class Vector;
class CCSInventoryManager;
class CCSPlayerController;

inline std::uint64_t(__fastcall* fnFindKeyVar)(const char*, unsigned int, int);
struct MaterialKeyVar_t
{
	std::uint64_t uKey;
	const char* szName;

	MaterialKeyVar_t(std::uint64_t uKey, const char* szName)
		: uKey(uKey), szName(szName)
	{}

	MaterialKeyVar_t(const char* szName, bool bShouldFindKey = false)
		: szName(szName)
	{
		uKey = bShouldFindKey ? FindKey(szName) : 0x0;
	}

	std::uint64_t FindKey(const char* szName)
	{
		return fnFindKeyVar ? fnFindKeyVar(szName, 0x12, 0x31415926) : 0;
	}
};

namespace memory
{
	void Initialize();
	void Shutdown();
	inline int(__cdecl* fnRandomSeed)(int iSeed) = nullptr;
	inline int(__cdecl* fnRandomInt)(int iMinValue, int iMaxValue) = nullptr;
	inline void* (__thiscall* fnGetBaseEntity)(void*, int);
	inline int(__thiscall* fnGetHighestEntityIndex)(void*, int*);
	inline void* (__thiscall* schema_detailed_class_layout)(void*, const char*);
	inline void* fnMouseInputEnabled;
	inline void(__thiscall* fnSetMeshGroupMask)(void*, uint64_t);
	inline CCSInventoryManager* (*fnGetInventoryManager)();
	inline void* fnInput;
	inline CGCClientSystem* (*fnGetClientSystem)();
	inline CEconItem* (*fnCreateSharedObjectSubclassEconItem)();
	inline CGCClientSharedObjectTypeCache* (__thiscall* fnCreateBaseTypeCache)(
		void*, int);
	inline CGCClientSharedObjectCache* (__thiscall* fnFindSOCache)(void*, SOID_t,
		bool);
	inline CCSPlayerController* (__fastcall* fnGetLocalPlayerController)(int);
	inline void* (__fastcall* fnSetDynamicAttributeValueUint)(void*, void*,
		void*);
	inline void* (__fastcall* fnSetModel)(void*, const char*);
	inline bool(__fastcall* fnComputeHitboxSurroundingBox)(void*, Vector&,
		Vector&);
	inline void* fnGetMatricesForView;
	inline void(__fastcall* hkUpdateSky3DParams)(void*);
	inline void* UpdateSkyboxColor;
	inline void* m_CSGOInput_SetViewAngles;
	inline void* m_CSGOInput_GetViewAngles;
	inline void* GlowFunction;

	inline void(__fastcall* fnGetPlayerPawn)(void*);

	 
	  

	inline void* hkApplyColorToSceneObject;
	inline void* hksetup_lighting;

	inline void* fnFireEventClientSide;
	inline void* fnCreatMove;
	inline void* fnFrameStage;
	 
	 
	 

	 
	inline void(__stdcall* fnSDL_SetRelativeMouseMode)(void*, bool);
	inline bool(__stdcall* fnSDL_GetRelativeMouseMode)(void*);
	inline void* (__stdcall* fnSDL_GetGrabbedWindow)();
	inline int(__stdcall* fnSDL_SetWindowGrab)(void*, int);
	inline int(__stdcall* fnSDL_WarpMouseInWindow)(void*, float, float);

	 
	inline void(__fastcall* fnSetMaterial)(void*, MaterialKeyVar_t, int, int);
	 
	 
	inline void(__fastcall* fnSetMaterialShaderType)(void*, MaterialKeyVar_t,
		const char*, int);
	inline std::int64_t(__fastcall* fnCreateMaterial)(void*, void*,
		const char*, void*,
		unsigned int,
		unsigned int);
	inline void(__fastcall* fnDelete)(void*, int, const char*, int);

	inline void* (__fastcall* fnAllocMem)(int64_t);
	inline void(__fastcall* fnDeAllocMem)(void*);

	inline bool(__fastcall* fnTraceShape)(void*, void*, Vector*, Vector*, void*, void*);
	inline bool(__fastcall* fnClipRayToEntity)(void*, void*, Vector*, Vector*, void*, void*, void*);

	inline  void* (__fastcall* fnGetSurfaceData)(void*);

	 
}   

