#include "../sdk/memory/command_crc.hpp"
#include <vector>
#include <algorithm>
#include "../utils/world_colors.hpp"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../sdk/interfaces/interfaces.hpp"
#include "../api/hook/hook.hpp"
 
#include "menu/menu.hpp"
#include "movement/movement.hpp"
#include "esp/esp.hpp"
#include "chams/chams.hpp"
#include <intrin.h>
#include "world/world.hpp"
#include "external_aim/aim.h"
#include "skins/knife.h"

static void set_color(void* data, int r, int g, int b, int a)
{
	*(byte*)((uintptr_t)data + 0x40) = r;
	*(byte*)((uintptr_t)data + 0x41) = g;
	*(byte*)((uintptr_t)data + 0x42) = b;
	*(byte*)((uintptr_t)data + 0x43) = a;
}
static CHook<bool __fastcall(void*)> g_mouseInputEnabled;
static bool __fastcall hkMouseInputEnabled(void* rcx)
{
	if (menu::IsOpen())
		return false;

	return g_mouseInputEnabled(rcx);
}

static CHook<void* __fastcall(void*, bool)> oIsRelativeMouseMode;
static void* IsRelativeMouseMode(void* pThisptr, bool bActive)
{
	 

	if (menu::IsOpen())
		return oIsRelativeMouseMode(pThisptr, false);

	return oIsRelativeMouseMode(pThisptr, bActive);
}

static CHook<void* __fastcall(void*, CEntityInstance*, CHandle)> g_onAddEntity;
static void* __fastcall hkOnAddEntity(void* rcx, CEntityInstance* pInstance,
	CHandle hHandle)
{
	auto* result = g_onAddEntity(rcx, pInstance, hHandle);
	visuals::OnAddEntity(pInstance, hHandle);
	return result;
}
static CHook<void* __fastcall(void*, CEntityInstance*, CHandle)>
g_onRemoveEntity;
static void* __fastcall hkOnRemoveEntity(void* rcx, CEntityInstance* pInstance,
	CHandle hHandle)
{
	visuals::OnRemoveEntity(pInstance, hHandle);
	return g_onRemoveEntity(rcx, pInstance, hHandle);
}
static CHook<void __fastcall(void*, void*, VMatrix*, VMatrix*, VMatrix*,
	VMatrix*)>
	g_getMatricesForView;
static void __fastcall hkGetMatricesForView(void* rcx, void* view,
	VMatrix* pWorldToView,
	VMatrix* pViewToProjection,
	VMatrix* pWorldToProjection,
	VMatrix* pWorldToPixels)
{
	g_getMatricesForView(rcx, view, pWorldToView, pViewToProjection,
		pWorldToProjection, pWorldToPixels);

	math::UpdateViewMatrix(pWorldToProjection);
	visuals::CalculateBoundingBoxes();
}

 
	 
	 
 

static CHook<bool __fastcall(CCSGOInput*, int, bool)> g_CreateMove;
static void RunTrigger(C_CSPlayerPawn* local, CUserCmd* command)
{
    static CHandle target;
    static ULONGLONG acquired = 0;
    if (!aim::trigger || menu::IsOpen() || local->m_iHealth() <= 0 || (aim::isHotTrigger && !(GetAsyncKeyState(VK_MENU) & 0x8000)) || !interfaces::pTraceMgr || !memory::fnTraceShape) {
        target = CHandle{}; acquired = 0; return;
    }
    Vector forward;
    math::angle_vectors(command->pBase.pBase->pViewangles->angValue, &forward, nullptr, nullptr);
    Vector start = local->GetEyePosition(), end = start + forward * 8192.f;
    Ray_t ray{}; GameTrace_t trace{}; TraceFilter_t filter(0x1C3003, local, nullptr, 4);
    if (!interfaces::pTraceMgr->TraceShape(&ray, start, end, &filter, &trace)) { target = CHandle{}; return; }
    auto* hit = trace.m_pHitEntity;
    auto* info = hit ? hit->Schema_DynamicBinding() : nullptr;
    if (!info || !info->GetName() || strcmp(info->GetName(), "C_CSPlayerPawn") || hit == local || hit->m_iHealth() <= 0 || !hit->IsEnemyWithTeam(local->m_iTeamNum())) { target = CHandle{}; return; }
    const auto handle = hit->GetRefEHandle(); const auto now = GetTickCount64();
    if (!(target == handle)) { target = handle; acquired = now; }
    if (now - acquired >= static_cast<ULONGLONG>(std::clamp(aimConf.trigger_reaction, 0, 400))) {
        command->buttonStates.nButtonState1 |= IN_ATTACK;
        command->buttonStates.nButtonState2 |= IN_ATTACK;
    }
}

static bool __fastcall hkCreateMove(CCSGOInput* pInput, int nSlot, bool active)
{
	const bool result = g_CreateMove(pInput, nSlot, active);
    if (nSlot != 0) return result;
    CUserCmd* pCmd = pInput->GetUserCmd();
	if (!interfaces::pEngine->IsInGame() || !interfaces::pEngine->IsConnected())
		return result;

	if (!pCmd)
		return result;

	auto basecmd = pCmd->pBase.pBase;
	if (!basecmd || !basecmd->pViewangles)
		return result;

	CCSPlayerController* LocalController = CGameEntitySystem::GetLocalPlayerController();
	if (LocalController == nullptr)
		return result;

	C_CSPlayerPawn* LocalPawn = LocalController->m_hPawn().Get<C_CSPlayerPawn>();
	if (LocalPawn == nullptr)
		return result;

	CGameEntitySystem* pEntitySystem = CGameEntitySystem::GetInstance();
	if (!pEntitySystem)
		return result;

	if (auto* sensitivity = interfaces::pCvar->FindVarByName("sensitivity")) aim::sens = sensitivity->GetValue<float>();
	

    C_CSPlayerPawn* bestTarget = nullptr;
    uintptr_t bestBones = 0;
    float bestFov = std::max(aim::fov, 0.f);
    const bool aiming = aim::state && !menu::IsOpen() && LocalPawn->m_iHealth() > 0 && (aim::alaimlock || (GetAsyncKeyState(VK_LBUTTON) & 0x8000));
    const int boneIndices[] = {6, 5, 4, 0};
    if (!aiming) aim::lockedPlayer = nullptr;
	for (int i = 1, highest = pEntitySystem->GetHighestEntityIndex(); i <= highest; ++i)
	{
		C_BaseEntity* pEntity = pEntitySystem->GetBaseEntity(i);
		if (!pEntity)
			continue;

		SchemaClassInfoData_t* pClassInfo = pEntity->Schema_DynamicBinding();
		if (pClassInfo == nullptr || !pClassInfo->GetName())
			continue;

		if (std::string_view{ pClassInfo->GetName() }.find("CCSPlayerController") == std::string::npos)
			continue;

		CCSPlayerController* pPlayerController = reinterpret_cast<CCSPlayerController*>(pEntity);
		if (!pPlayerController->m_bPawnIsAlive())
			continue;

		C_CSPlayerPawn* pPawn = pPlayerController->m_hPawn().Get<C_CSPlayerPawn>();
		if (!pPawn)
			continue;

		const bool isLocalPlayer = pPlayerController->m_bIsLocalPlayerController();
		const bool isEnemy = pPawn->IsEnemyWithTeam(LocalController->m_iTeamNum());
		if (LocalController->m_iTeamNum() == pPawn->m_iTeamNum() || !isEnemy)
			continue;

		if (aim::lockedPlayer == LocalPawn && (LocalPawn->m_iHealth() <= 0 || (aim::checkSpotted && !LocalPawn->getEntitySpotted()))) aim::lockedPlayer = 0;
		if ((LocalPawn->m_iHealth() <= 0 || LocalPawn->m_iHealth() > 100) || (LocalController->m_sSanitizedPlayerName() && strcmp(LocalController->m_sSanitizedPlayerName(), "DemoRecorder") == 0)) continue;

		auto gameSceneNode = pPawn->m_pGameSceneNode();
		if (!gameSceneNode)
			continue;
		 

        if (aiming && aim::boneSelect >= 0 && aim::boneSelect < 4) {
            if (aim::playerLock && aim::lockedPlayer && aim::lockedPlayer != pPawn) continue;
            const auto bones = gameSceneNode->getBoneArray();
            if (!bones) continue;
            const Vector point = reinterpret_cast<Matrix2x4_t*>(bones)->GetOrigin(boneIndices[aim::boneSelect]);
            auto delta = (point - LocalPawn->GetEyePosition()).ToAngle();
            delta.x -= basecmd->pViewangles->angValue.x; delta.y -= basecmd->pViewangles->angValue.y;
            delta.Normalize(); const float distance = std::hypot(delta.x, delta.y);
            if (!std::isfinite(distance) || distance > bestFov || (aim::checkSpotted && !LocalPawn->IsVisible(pPawn, point, false, 0.f))) continue;
            bestFov = distance; bestTarget = pPawn; bestBones = bones;
        }
	}
    aim::recoilControl(aim::rcs && !menu::IsOpen() && !bestTarget, LocalPawn);
    if (bestTarget) aim::aimBot(bestTarget, bestBones, pCmd, basecmd); else aim::lockedPlayer = nullptr;
    RunTrigger(LocalPawn, pCmd);
    if (basecmd->pInButtonState && (basecmd->pInButtonState->State1 != pCmd->buttonStates.nButtonState1 || basecmd->pInButtonState->State2 != pCmd->buttonStates.nButtonState2 || basecmd->pInButtonState->State3 != pCmd->buttonStates.nButtonState3)) {
        *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(basecmd) + 0x10) |= 4u;
        *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(basecmd->pInButtonState) + 0x10) |= 7u;
        basecmd->pInButtonState->State1=pCmd->buttonStates.nButtonState1;
        basecmd->pInButtonState->State2=pCmd->buttonStates.nButtonState2;
        basecmd->pInButtonState->State3=pCmd->buttonStates.nButtonState3;
        if (!CRC::UpdateMoveCRC(pCmd)) LOG("Command CRC update failed.\n");
    }
	return result;
}

static CMaterial2* visible = nullptr;
static CMaterial2* visible_black = nullptr;
static CMaterial2* invisible_black = nullptr;
static CMaterial2* invisible = nullptr;
static CMaterial2* visible_bloom = nullptr;

void __fastcall hkDrawObject(void* descriptor,void* context,CMaterial2_Data_t* data,int count,void* view,void* layer,void* extra,void* extra2) {
 std::vector<CMaterial2_Data_t> filtered;
 if (menu::legscheck && data && count > 0 && count <= 4096) {
  auto* controller = CGameEntitySystem::GetLocalPlayerController();
  auto* local = controller ? controller->m_hPawn().Get<C_CSPlayerPawn>() : nullptr;
  if (local) {
   filtered.reserve(count);
   for (int i=0;i<count;++i) {
    auto* object = data[i].m_pSceneAnimatableObject;
    if (!object || object->hOwner.Get() != local) filtered.push_back(data[i]);
   }
   if (filtered.empty()) return;
   data = filtered.data(); count = static_cast<int>(filtered.size());
  }
 }
 auto original = [&] { g_DrawObject(descriptor,context,data,count,view,layer,extra,extra2); };
 if (!data || count <= 0 || count > 4096 || !interfaces::pEngine || !interfaces::pEngine->IsInGame()) { original(); return; }
 auto* controller = CGameEntitySystem::GetLocalPlayerController();
 auto* local = controller ? controller->m_hPawn().Get<C_CSPlayerPawn>() : nullptr;
 if (!local || (!chams::player && !chams::invisible && !chams::weapon && !chams::hands)) { original(); return; }
 if (!chams::chams_init && !chams::init()) { original(); return; }
 struct Saved { CMaterial2* material; CMaterial2* secondary; cmyk_color_t color; bool change; };
 std::vector<Saved> saved; saved.reserve(count);
 for (int i=0;i<count;++i) saved.push_back({data[i].m_material,data[i].secondary,data[i].color,false});
 struct Restore {
  CMaterial2_Data_t* data; std::vector<Saved>& saved;
  void Run() { for (size_t i=0;i<saved.size();++i) { data[i].m_material=saved[i].material; data[i].secondary=saved[i].secondary; data[i].color=saved[i].color; saved[i].change=false; } }
  ~Restore() { Run(); }
 } restore{data,saved};
 auto apply = [&](bool hidden) {
  bool any=false;
  for (int i=0;i<count;++i) {
   auto* object=data[i].m_pSceneAnimatableObject; auto* entity=object ? object->hOwner.Get() : nullptr;
   auto* info=entity ? entity->Schema_DynamicBinding() : nullptr;
   const char* name=info ? info->GetName() : nullptr;
   if (!name) continue;
   CMaterial2* material=nullptr;int intensity=50;
   if (!strcmp(name,"C_CSPlayerPawn") && entity != local && ((hidden && chams::invisible) || (!hidden && chams::player)) && (chams::bChams_team || entity->m_iTeamNum()!=local->m_iTeamNum())) {
    if (hidden && !chams::invisible) continue;
    intensity=hidden ? chams::invisible_value : chams::visible_value;
    material=hidden ? (intensity>30?chams::invisibleC:chams::invisibleforblackC) : (chams::bloom?chams::bloomC:(intensity>30?chams::visibleC:chams::visibleforblackC));
   } else if (!hidden && chams::weapon && (!strcmp(name,"C_CS2HudModelWeapon") || entity->IsBasePlayerWeapon())) { material=chams::visibleC;intensity=chams::weapon_value; }
   else if (!hidden && chams::hands && !strcmp(name,"C_CS2HudModelArms")) { material=chams::visibleC;intensity=chams::hands_value; }
   if (!material) continue;
   auto color=static_cast<uint8_t>(std::clamp(intensity,0,255));
   data[i].m_material=material;data[i].secondary=material;data[i].color={color,color,color,255};saved[i].change=true;any=true;
  }
  return any;
 };
 if (apply(true)) original();
 restore.Run();
 apply(false);original();
}

static CHook<bool __fastcall(CGlowProperty*)> g_Glow;
bool __fastcall hkGlow(CGlowProperty* GlowProperty)
{
	if (!other::glow)
		return g_Glow(GlowProperty);

	if (!GlowProperty)
		return g_Glow(GlowProperty);

	if (!GlowProperty->m_hOwner)
		return g_Glow(GlowProperty);

	CEntityInstance* owner = GlowProperty->m_hOwner;
	if (!owner)
		return g_Glow(GlowProperty);

	auto* info = owner->Schema_DynamicBinding();
	if (!info || !info->GetName() || strcmp(info->GetName(), "C_CSPlayerPawn") != 0) return g_Glow(GlowProperty);
	auto* PlayerPawn = static_cast<C_CSPlayerPawn*>(owner);

	if (!PlayerPawn || PlayerPawn->m_iHealth() < 1)
		return g_Glow(GlowProperty);

	CCSPlayerController* LocalController = CGameEntitySystem::Get().GetLocalPlayerController();
	if (!LocalController)
		return g_Glow(GlowProperty);

	C_CSPlayerPawn* LocalPawn = LocalController->m_hPawn().Get<C_CSPlayerPawn>();
	if (!LocalPawn)
		return g_Glow(GlowProperty);

	if ((LocalPawn == PlayerPawn) || (LocalPawn->m_iTeamNum() == PlayerPawn->m_iTeamNum()))
		return g_Glow(GlowProperty);

	Color_t glowCol = Color_t(other::intesivity, other::intesivity, other::intesivity);

	GlowProperty->Glowing() = true;
	GlowProperty->GlowColorOverride() = glowCol;

	return g_Glow(GlowProperty);
}

static CHook<void __fastcall(void*, int)> g_FrameStageNotify;
void __fastcall hkFrameStageNotify(void* rcx, int stage)
{
    g_FrameStageNotify(rcx, stage);
    if (stage == 6) {
        static auto** traceSlot = CModule(CLIENT_DLL).FindPattern(SDK_SIG("4C 8B 35 ? ? ? ? 24 ? 0C ? 66 0F 7F 45 ? 88 45 ? 48 8B CB 48 8D 05 ? ? ? ? 89 7D ? 48 89 45 ? 89 7D ? C7 45 ? ? ? ? ? 66 C7 45 ? ? ? 44 88 7D")).ToAbsolute(3, 0).Get<CGameTraceManager**>();
        interfaces::pTraceMgr = traceSlot ? *traceSlot : nullptr;
    }
    if (stage == 6 && interfaces::pEngine->IsInGame()) {
        static CHandle previous;
        static float flashAlpha = 255.f;
        static bool applied = false;
        auto* controller = CGameEntitySystem::GetLocalPlayerController();
        auto* pawn = controller ? controller->m_hPawn().Get<C_CSPlayerPawn>() : nullptr;
        if (pawn) {
            auto handle = pawn->GetRefEHandle();
            if (!(previous == handle)) { previous = handle; applied = false; }
            if (menu::flashcheck) {
                if (!applied) flashAlpha = pawn->m_flFlashMaxAlpha();
                pawn->m_flFlashMaxAlpha() = 0.f; applied = true;
            } else if (applied) { pawn->m_flFlashMaxAlpha() = flashAlpha; applied = false; }
        }
    }
	switch (stage) {
	case 6:
		knife_changer::Run();
		break;
	}

	if (stage == 6 && interfaces::pEngine->IsInGame()) world::SkyBoxChanger();
	 
	 
	    
		    
	 

	 
	 
	 
	 

}

static CHook<void* __fastcall(void*)>g_3DBox;
void* __fastcall UpdateSky3DParams(void* a1)
{
	if (world::skybox)
		return nullptr;

	return g_3DBox(a1);
}

struct AggregateRange { uint32_t reserved; int32_t count; char pad[0x30]; int32_t index; };
struct AggregateData { void* object; AggregateRange* range; };
struct ColorQueue { void* reserved; std::byte* limit; void* reserved2; std::byte* colors; };
static WorldColors::Cache worldColorCache;
static CHook<void __fastcall(void*,void*,AggregateData*)> g_ApplyColorSO;
void ApplyColorToSceneObject(void* scene,void* context,AggregateData* data) {
 g_ApplyColorSO(scene,context,data);
 if (!data || !data->range || data->range->count <= 0 || data->range->index < 0) return;
 static auto** slot = CModule(SCENESYSTEM_DLL).FindPattern(SDK_SIG("4C 8B 35 ? ? ? ? C1 E0 05 48 63 D0 49 03 56 18 49 3B 56 08 76 ? 49 8B CE FF 15 ? ? ? ? 84 C0 75 ? 49 8B F4 BB FF FF FF FF EB")).ToAbsolute(3,0).Get<ColorQueue**>();
 auto* queue = slot ? *slot : nullptr;
 if (!queue || !queue->colors || !queue->limit || queue->limit < queue->colors) return;
 const auto capacity = static_cast<size_t>(queue->limit - queue->colors) / 0x20;
 worldColorCache.Range(queue->colors,capacity,data->range->index,data->range->count,world::nightmode,{40,40,40});
}

float g_lighting_color[3] = { 255.f / 255.f, 255.f / 255.f, 255.f / 255.f };
float g_lighting_brightness = 6.f;
static CHook<void __fastcall(void*,void*,int64_t)> g_SetupLight;
void setup_lighting(void* binner,void* light,int64_t context) {
 if (!light || !world::nightmode) { g_SetupLight(binner,light,context); return; }
 auto* color = reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(light) + 0xe4);
 float saved[3] = {color[0],color[1],color[2]};
 for (int i=0;i<3;++i) color[i] = g_lighting_color[i] * g_lighting_brightness;
 g_SetupLight(binner,light,context);
 for (int i=0;i<3;++i) color[i] = saved[i];
}

void HookGameFunctions()
{
	g_mouseInputEnabled.Hook(memory::fnMouseInputEnabled, HOOK_FUNCTION(hkMouseInputEnabled));
	oIsRelativeMouseMode.HookVirtual(interfaces::pInputSystem, 76, HOOK_FUNCTION(IsRelativeMouseMode));
	g_onAddEntity.Hook(CModule(CLIENT_DLL).FindPattern(SDK_SIG("48 89 74 24 ? 57 48 83 EC ? 41 B9 ? ? ? ? 41 8B C0 41 23 C1 48 8B F2 41 83 F8 ? 48 8B F9 44 0F 45 C8 41 81 F9 ? ? ? ? 73 ? FF 81")).Get<void*>(), HOOK_FUNCTION(hkOnAddEntity));
	g_onRemoveEntity.Hook(CModule(CLIENT_DLL).FindPattern(SDK_SIG("48 89 74 24 ? 57 48 83 EC ? 41 B9 ? ? ? ? 41 8B C0 41 23 C1 48 8B F2 41 83 F8 ? 48 8B F9 44 0F 45 C8 41 81 F9 ? ? ? ? 73 ? FF 89")).Get<void*>(), HOOK_FUNCTION(hkOnRemoveEntity));
	g_getMatricesForView.Hook(memory::fnGetMatricesForView, HOOK_FUNCTION(hkGetMatricesForView));
	g_CreateMove.Hook(memory::fnCreatMove, HOOK_FUNCTION(hkCreateMove));
	g_DrawObject.Hook(fnDrawObject, HOOK_FUNCTION(hkDrawObject));
	g_FrameStageNotify.Hook(memory::fnFrameStage, HOOK_FUNCTION(hkFrameStageNotify));
	 
	g_ApplyColorSO.Hook(memory::hkApplyColorToSceneObject, HOOK_FUNCTION(ApplyColorToSceneObject));
	g_SetupLight.Hook(memory::hksetup_lighting, HOOK_FUNCTION(setup_lighting));

	g_Glow.Hook(memory::GlowFunction, HOOK_FUNCTION(hkGlow));

	visuals::CacheCurrentEntities();
}
