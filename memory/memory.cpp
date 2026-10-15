#include "../../utils/native_rtti.hpp"
#include "memory.hpp"
#include <stb/stb.hh>
#include "../interfaces/interfaces.hpp"
#define MEMORY_VARIABLE(var) var, "memory::" #var

void memory::Initialize()
{
	CModule client(CLIENT_DLL);
	CModule schemasystem(SCHEMASYSTEM_DLL);
	CModule sdl3(SDL3_DLL);
	CModule tier0(TIER0_DLL);
	CModule particles(PARTICLES_DLL);
	CModule scenesystem(SCENESYSTEM_DLL);
	CModule materialsystem(MATERIALSYSTEM_DLL);

	schemasystem.FindPattern(PRINT_SCHEMA_DETAILED_CLASS_LAYOUT)
		.Get(MEMORY_VARIABLE(schema_detailed_class_layout));

	client.FindPattern(MOUSE_INPUT_ENABLED)
		.Get(MEMORY_VARIABLE(fnMouseInputEnabled));


	client.FindPattern(GET_GC_CLIENT_SYSTEM)
		.ToAbsolute(1, 0)
		.Get(MEMORY_VARIABLE(fnGetClientSystem));


	client.FindPattern(CREATE_BASE_TYPE_CACHE)
		.ToAbsolute(1, 0)
		.Get(MEMORY_VARIABLE(fnCreateBaseTypeCache));

	client.FindPattern(FIND_SO_CACHE)
		.ToAbsolute(1, 0)
		.Get(MEMORY_VARIABLE(fnFindSOCache));

    client.FindPattern(SDK_SIG("85 D2 0F 85 ? ? ? ? 48 89 5C 24 08 57 48 83 EC 20 48 8B F9 49 8B D8 8B CA E8 ? ? ? ? 48 85 C0 0F 84 ? ? ? ? 48 8D 54"))
        .AddOffset(0x1A).ToAbsolute(1, 0).Get(MEMORY_VARIABLE(fnGetLocalPlayerController));

	client.FindPattern(SET_DYNAMIC_ATTRIBUTE_VALUE_UINT)
		.ToAbsolute(1, 0)
		.Get(MEMORY_VARIABLE(fnSetDynamicAttributeValueUint));


	client.FindPattern(COMPUTE_HITBOX_SURROUNDING_BOX)
		.ToAbsolute(1, 0)
		.Get(MEMORY_VARIABLE(fnComputeHitboxSurroundingBox));

	client.FindPattern(GET_MATRICES_FOR_VIEW)
		.Get(MEMORY_VARIABLE(fnGetMatricesForView));

	 

	client.FindPattern(SDK_SIG("48 89 54 24 10 48 89 4C 24 08 55 53 56 57 41 54 41 56 41 57 48 8D AC 24 ? ? ? ? B8 10 25 00 00")).Get(MEMORY_VARIABLE(fnTraceShape));
	client.FindPattern(SDK_SIG("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 54 41 56 41 57 48 81 EC ? ? ? ? 48 8B 9C 24")).Get(MEMORY_VARIABLE(fnClipRayToEntity));

	client.FindPattern(SDK_SIG("4C 8B DC 53 48 81 EC ? ? ? ? 48 8B 41")).Get(MEMORY_VARIABLE(fnGetPlayerPawn));

	client.FindPattern(SDK_SIG("48 63 41 ? 48 8B 0D")).Get(MEMORY_VARIABLE(fnGetSurfaceData));

	client.FindPattern(SDK_SIG("4C 8B 35 ? ? ? ? 24 ? 0C ? 66 0F 7F 45 ? 88 45 ? 48 8B CB 48 8D 05 ? ? ? ? 89 7D ? 48 89 45 ? 89 7D ? C7 45 ? ? ? ? ? 66 C7 45 ? ? ? 44 88 7D")).ToAbsolute(0x3, 0).Dereference(1).Get(interfaces::pTraceMgr);


    using input_t = void* (__fastcall*)();
    auto getInput = client.FindPattern(SDK_SIG("E8 ? ? ? ? 48 8B 56 ? 48 8B C8 E8 ? ? ? ? 4C 89 7E")).ToAbsolute(1, 0).Get<input_t>();
    fnInput = getInput ? getInput() : nullptr;
    if (fnInput) fnCreatMove = (*reinterpret_cast<void***>(fnInput))[5];
    if (interfaces::pClient) fnFrameStage = (*reinterpret_cast<void***>(interfaces::pClient))[36];

	 
	 

 client.FindPattern(SDK_SIG("85 D2 75 ? 48 63 81")).Get(m_CSGOInput_SetViewAngles);
 client.FindPattern(SDK_SIG("4C 8B C1 85 D2 74 08 48 8D 05 ? ? ? ? C3")).Get(m_CSGOInput_GetViewAngles);

	client.FindPattern(SKYBOX_COLOR_UPDATE)
		.Get(MEMORY_VARIABLE(UpdateSkyboxColor));

	client.FindPattern(SDK_SIG("0F B6 41 51 C3"))
		.Get(MEMORY_VARIABLE(GlowFunction));

	client.FindPattern(SDK_SIG("40 53 48 83 EC 20 48 8B D9 4C 8B C2 48 8B 0D ? ? ? ? 48 8D 54 24"))
		.Get(MEMORY_VARIABLE(fnSetModel));

	tier0.GetProcAddress("RandomInt").Get(MEMORY_VARIABLE(fnRandomInt));
	tier0.GetProcAddress("RandomSeed").Get(MEMORY_VARIABLE(fnRandomSeed));
	tier0
		.GetProcAddress(
			"?LoadKV3@@YA_NPEAVKeyValues3@@PEAVCUtlString@@PEAVCUtlBuffer@@AEBUKV3ID_t@@PEBDI@Z")
		.Get(MEMORY_VARIABLE(fnLoadKV3));

tier0.FindPattern(SDK_SIG("40 53 48 83 EC 30 80 FA 06 0F B6 C2 41 B9 16")).Get(MEMORY_VARIABLE(fnSetType));

	client.FindPattern(FREE_ALLOC_KV3).ToAbsolute(1, 0).Get(
		MEMORY_VARIABLE(pFnFreeAlloc));

	particles.FindPattern(SET_MATERIAL_SHADER)
		.ToAbsolute(32, 0)
		.Get(MEMORY_VARIABLE(fnSetMaterialShaderType));

	 

	particles.FindPattern(FIND_VAR).Get(MEMORY_VARIABLE(fnFindKeyVar));

	auto** drawTable = NativeRtti::FindVTable(GetModuleHandleA(SCENESYSTEM_DLL),".?AVCAnimatableSceneObjectDesc@@");
    if (drawTable) fnDrawObject = reinterpret_cast<decltype(fnDrawObject)>(drawTable[1]);

 scenesystem.FindPattern(SDK_SIG("48 8B C4 48 89 50 ? 48 89 48 ? 55 53 56 57 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 70")).Get(hkApplyColorToSceneObject);
 scenesystem.FindPattern(SDK_SIG("48 89 54 24 ? 55 57 41 56 48 83 EC")).Get(hksetup_lighting);

	 
		 

	tier0.GetProcAddress("??3KeyValues@@SAXPEAXHPEBDH@Z")
		.Get(MEMORY_VARIABLE(fnDelete));

	tier0.GetProcAddress("MemAlloc_AllocFunc")
		.Get(MEMORY_VARIABLE(fnAllocMem));

	tier0.GetProcAddress("MemAlloc_FreeFunc")
		.Get(MEMORY_VARIABLE(fnDeAllocMem));

	sdl3.GetProcAddress("SDL_SetWindowRelativeMouseMode").Get(fnSDL_SetRelativeMouseMode);
	sdl3.GetProcAddress("SDL_GetWindowRelativeMouseMode").Get(fnSDL_GetRelativeMouseMode);
	sdl3.GetProcAddress("SDL_GetGrabbedWindow").Get(fnSDL_GetGrabbedWindow);
	sdl3.GetProcAddress("SDL_SetWindowMouseGrab").Get(fnSDL_SetWindowGrab);
	sdl3.GetProcAddress("SDL_WarpMouseInWindow").Get(fnSDL_WarpMouseInWindow);

	UTILPtr ppHeapMemAlloc = tier0.GetProcAddress("g_pMemAlloc");
	if (ppHeapMemAlloc.IsValid())
		ppHeapMemAlloc.Dereference(1).Get(MEMORY_VARIABLE(g_pHeapMemAlloc));
 materialsystem.FindPattern(SDK_SIG("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 8B F2")).Get(fnCreateMaterial);
 if (!fnCreateMaterial && interfaces::pMaterialSystem) fnCreateMaterial = reinterpret_cast<decltype(fnCreateMaterial)>((*reinterpret_cast<void***>(interfaces::pMaterialSystem))[29]);
 client.FindPattern(SDK_SIG("48 8D 05 ? ? ? ? C3 CC CC CC CC CC CC CC CC 8B 91 ? ? ? ? B8")).Get(fnGetInventoryManager);
 client.FindPattern(SDK_SIG("48 83 EC 28 B9 48 00 00 00 E8 ? ? ? ? 48 85")).Get(fnCreateSharedObjectSubclassEconItem);

}

void memory::Shutdown()
{}

