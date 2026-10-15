#include <vector>
#include <mutex>
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../render/render.hpp"
#include "esp.hpp"
#include "../menu/menu.hpp"

#include "../../sdk/interfaces/interfaces.hpp"

static CCSPlayerController* g_pLocalPlayerController = nullptr;
static C_CSPlayerPawn* g_pLocalPlayerPawn = nullptr;

 
struct CachedEntity_t
{
	enum Type
	{
		UNKNOWN = 0, PLAYER_CONTROLLER, BASE_WEAPON
	};

	CHandle m_handle;
	Type m_type = PLAYER_CONTROLLER;

	BBox_t m_bbox;
	bool m_draw = true;
};
static std::vector<CachedEntity_t> g_cachedEntities;
static std::mutex g_cachedEntitiesMutex;

static CachedEntity_t::Type GetEntityType(C_BaseEntity* pEntity);
static void RenderPlayerESP(CCSPlayerController* pPlayerController,
							const BBox_t& bBox);

static void RenderWeaponESP(C_CSWeaponBase* pWeapon, const BBox_t& bBox);
static void RenderWeaponName(C_CSWeaponBase* pWeapon, const BBox_t& bBox);

void FillRGBA(int x, int y, int w, int h, ImColor color)
{
	visuals::g_pBackgroundDrawList->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), color);
}

VOID DrawBox(int x, int y, int w, int h, int lw, ImColor color)
{
	FillRGBA(x, y, w, lw, color);  
	FillRGBA(x, y + lw, lw, h - lw, color);  
	FillRGBA(x + w - lw, y + lw, lw, h - lw, color);  
	FillRGBA(x + lw, y + h - lw, w - lw * 2, lw, color);  
}

VOID DrawHeader(INT x, INT y, INT w, ImColor color, INT HealthBarWidth)
{
	INT i = 0;
	INT xCoord = x;
	INT yCoord = y;

	 
	for (i = 0; i < 5; i++)
	{
		FillRGBA(x - i, y + i, w, 1, ImColor(0, 0, 0, 255));
		FillRGBA(x - i, y + i, HealthBarWidth + 2, 1, color);

		FillRGBA(x - 3, y + 3, HealthBarWidth + 2, 1, ImColor((INT) (color.Value.x / 1.5f),
				 (INT) (color.Value.y / 1.5f), (INT) (color.Value.z / 1.5f), 255));

		FillRGBA(x - 4, y + 4, HealthBarWidth + 2, 1, ImColor((INT) (color.Value.x / 1.5f),
				 (INT) (color.Value.y / 1.5f), (INT) (color.Value.z / 1.5f), 255));
	}

	 
	if (visuals::esp_glow)
		visuals::g_pBackgroundDrawList->AddShadowRect(ImVec2(x, y), ImVec2(x + w, y + 1), ImColor(255, 255, 255, 255), 35.f, ImVec2(0, 0));

	FillRGBA(x, y, w, 1, ImColor(255, 255, 255, 255));

	 
	if (visuals::esp_glow)
		visuals::g_pBackgroundDrawList->AddShadowRect(ImVec2((x + 1) - 5, y + 5), ImVec2(((x + 1) - 5) + w, (y + 5) + 1), ImColor(255, 255, 255, 255), 35.f, ImVec2(0, 0));

	FillRGBA((x + 1) - 5, y + 5, w, 1, ImColor(255, 255, 255, 255));

	for (i = 0; i < 5; i++)
	{
		 
		if (visuals::esp_glow)
			visuals::g_pBackgroundDrawList->AddShadowRect(ImVec2(x, y), ImVec2(x + 1, y + 1), ImColor(255, 255, 255, 255), 15.f, ImVec2(0, 0));

		FillRGBA(x, y, 1, 1, ImColor(255, 255, 255, 255));
		x--;
		y++;
	}

	x = xCoord;
	y = yCoord;

	for (i = 0; i < 5; i++)
	{
		 
		if (i != 0)
		{
			if (visuals::esp_glow)
				visuals::g_pBackgroundDrawList->AddShadowRect(ImVec2(x + w, y), ImVec2(x + w + 1, y + 1), ImColor(255, 255, 255, 255), 15.f, ImVec2(0, 0));

			FillRGBA(x + w, y, 1, 1, ImColor(255, 255, 255, 255));
		}

		x--;
		y++;
	}

}

VOID DrawBoxHealth(int x, int y, int w, int h, int lw, ImColor color, int targetId, int damageDealt, int animTimer, int index)
{
	FillRGBA(x, y, w, lw, ImColor(0, 0, 0, 255));  
	FillRGBA(x, y + lw, lw, h - lw, ImColor(0, 0, 0, 255));  
	FillRGBA(x + w - lw, y + lw, lw, h - lw, ImColor(0, 0, 0, 255));  
	FillRGBA(x + lw, y + h - lw, w - lw * 2, lw, ImColor(0, 0, 0, 255));  

	if (color.Value.w == 0)
		color.Value.x = 50, color.Value.y = 50; color.Value.z = 50;

	FillRGBA(x + lw, y + lw, w - (lw * 2), h - (lw * 2), color);
}

void DrawOutlinedRect(int x0, int y0, int x1, int y1, ImColor color)
{
	int BoxWidth = x1 - x0;
	int BoxHeight = y1 - y0;

	if (BoxWidth < 10)
		BoxWidth = 10;

	if (BoxHeight < 15)
		BoxHeight = 15;

	visuals::g_pBackgroundDrawList->AddRectFilled(ImVec2(x0, y0), ImVec2(x0 + (BoxWidth / 5), y0 + 1), color);  
	visuals::g_pBackgroundDrawList->AddRectFilled(ImVec2(x0, y0), ImVec2(x0 + 1, y0 + (BoxHeight / 6)), color);  

	visuals::g_pBackgroundDrawList->AddRectFilled(ImVec2(x1 - (BoxWidth / 5) + 1, y0), ImVec2(x1, y0 + 1), color);  
	visuals::g_pBackgroundDrawList->AddRectFilled(ImVec2(x1, y0), ImVec2(x1 + 1, y0 + (BoxHeight / 6)), color);  

	visuals::g_pBackgroundDrawList->AddRectFilled(ImVec2(x0, y1), ImVec2(x0 + (BoxWidth / 5), y1 + 1), color);  
	visuals::g_pBackgroundDrawList->AddRectFilled(ImVec2(x0, y1 - (BoxHeight / 6) + 1), ImVec2(x0 + 1, y1 + 1), color);  

	visuals::g_pBackgroundDrawList->AddRectFilled(ImVec2(x1 - (BoxWidth / 5) + 1, y1), ImVec2(x1, y1 + 1), color);  
	visuals::g_pBackgroundDrawList->AddRectFilled(ImVec2(x1, y1 - (BoxHeight / 6) + 1), ImVec2(x1 + 1, y1 + 1), color);  
}

void DrawHealthBox(C_CSPlayerPawn* pPlayerPawn, int x, int y, int w)
{
	int CurHealth = pPlayerPawn->m_iHealth( );
	if (CurHealth > 100)
		CurHealth = 100;

	int iHealthWidth = (CurHealth * (w * 0.5 * 0.1) * 0.2);
	int iHealthMaxWidth = (100 * (w * 0.5 * 0.1) * 0.2);
	DrawHeader(x, y, iHealthMaxWidth + 2, ImColor(255, 255, 255, 255), iHealthWidth);
}

void visuals::Render( )
{
	if ((!esp_enable && !menu::bombcheck) || !interfaces::pEngine) return;
	if (!interfaces::pEngine->IsInGame( ))
		return;

	g_pLocalPlayerController = CGameEntitySystem::GetLocalPlayerController( );
	if (!g_pLocalPlayerController)
		return;

	g_pLocalPlayerPawn = g_pLocalPlayerController->m_hPawn( ).Get<C_CSPlayerPawn>( );
	if (!g_pLocalPlayerPawn)
		return;

	CGameEntitySystem* pEntitySystem = CGameEntitySystem::GetInstance( );
	if (!pEntitySystem)
		return;

	 
	for (int i = 1, highest = pEntitySystem->GetHighestEntityIndex(); i <= highest; ++i)
	{
		C_BaseEntity* pEntity = pEntitySystem->GetBaseEntity(i);
		if (!pEntity)
			continue;

		SchemaClassInfoData_t* pClassInfo = pEntity->Schema_DynamicBinding( );
		if (pClassInfo == nullptr || !pClassInfo->GetName())
			continue;

		if (menu::bombcheck && std::strcmp(pClassInfo->GetName(), "C_PlantedC4") == 0) {
            auto* node = pEntity->m_pGameSceneNode(); ImVec2 position;
            if (node && math::WorldToScreen(node->m_vecAbsOrigin(), position))
                render::text_outline(g_pBackgroundDrawList, position, IM_COL32(255, 180, 50, 255), "C4");
        }
        if (!esp_enable) continue;
        if (std::string_view{ pClassInfo->GetName( ) }.find("CCSPlayerController") == std::string::npos)
			continue;

		CCSPlayerController* pPlayerController = reinterpret_cast<CCSPlayerController*>(pEntity);
		if (!pPlayerController->m_bPawnIsAlive( ))
			continue;

		C_CSPlayerPawn* pPawn = pPlayerController->m_hPawn( ).Get<C_CSPlayerPawn>( );
		if (!pPawn)
			continue;

		const bool isLocalPlayer = pPlayerController->m_bIsLocalPlayerController( );
		const bool isEnemy = pPawn->IsEnemyWithTeam(g_pLocalPlayerController->m_iTeamNum( ));
		if (g_pLocalPlayerController->m_iTeamNum( ) == pPawn->m_iTeamNum( ) || !isEnemy)
			continue;

		BBox_t objBBox{};
		if (!pPawn->GetBoundingBox(objBBox, false))
			continue;

		CCollisionProperty* pCollision = pPawn->m_pCollision( );
		if (pCollision == nullptr)
			continue;

		CGameSceneNode* pGameSceneNode = pPawn->m_pGameSceneNode( );
		if (pGameSceneNode == nullptr)
			continue;

		 
		CSkeletonInstance* pSkeleton = reinterpret_cast<CSkeletonInstance*>(pGameSceneNode);
		if (pSkeleton == nullptr)
			continue;

		 
		Matrix2x4_t* pBoneCache = reinterpret_cast<Matrix2x4_t*>(pGameSceneNode->getBoneArray());
		if (pBoneCache == nullptr)
			continue;

		ImVec2 vScreen{};
		if (!math::WorldToScreen(pGameSceneNode->m_vecAbsOrigin( ), vScreen))
			continue;

		 
		Vector vecPos = pBoneCache->GetOrigin(6);
		Vector vTop = (Vector(pGameSceneNode->m_vecAbsOrigin( ).x, pGameSceneNode->m_vecAbsOrigin( ).y, vecPos.z + 7.0f));

		ImVec2 vTopScreen{};
		if (!math::WorldToScreen(vTop, vTopScreen))
			continue;

		float h = (vScreen.y - vTopScreen.y);
		float w = h / 4.5f;

		if (visuals::esp_box)
		{
			int r_x = vTopScreen.x - w;
			w *= 2;
			int frame_w = w;
			int frame_ww = frame_w;
			frame_ww /= 3;
			frame_w /= 2.3;

			ImColor colorShit = ImColor(15, 15, 15, 255);
			ImColor colorShit1 = ImColor(0, 0, 0, 255);

			int calc_x = r_x + (w - frame_ww);
			int calc_y = vTopScreen.y + (h - frame_w);

			if (visuals::esp_glow)
			{
				g_pBackgroundDrawList->AddShadowRect(ImVec2(r_x, vTopScreen.y - 1), ImVec2(r_x + frame_ww, (vTopScreen.y - 1) + 1), ImColor(255, 255, 255, 255), 30.f, ImVec2(0, 0));
				g_pBackgroundDrawList->AddShadowRect(ImVec2(r_x - 1, vTopScreen.y), ImVec2((r_x - 1) + 1, vTopScreen.y + frame_w), ImColor(255, 255, 255, 255), 30.f, ImVec2(0, 0));

				g_pBackgroundDrawList->AddShadowRect(ImVec2(calc_x, vTopScreen.y - 1), ImVec2(calc_x + frame_ww + 1, vTopScreen.y - 1 + 1), ImColor(255, 255, 255, 255), 30.f, ImVec2(0, 0));
				g_pBackgroundDrawList->AddShadowRect(ImVec2(r_x + w + 1, vTopScreen.y), ImVec2(r_x + w + 1 + 1, vTopScreen.y + frame_w), ImColor(255, 255, 255, 255), 30.f, ImVec2(0, 0));

				g_pBackgroundDrawList->AddShadowRect(ImVec2(r_x, vTopScreen.y + h + 1), ImVec2(r_x + frame_ww, vTopScreen.y + h + 1 + 1), ImColor(255, 255, 255, 255), 30.f, ImVec2(0, 0));
				g_pBackgroundDrawList->AddShadowRect(ImVec2(r_x - 1, calc_y), ImVec2(r_x - 1 + 1, calc_y + frame_w + 1), ImColor(255, 255, 255, 255), 30.f, ImVec2(0, 0));

				g_pBackgroundDrawList->AddShadowRect(ImVec2(calc_x + 1, vTopScreen.y + h + 1), ImVec2(calc_x + 1 + frame_ww, vTopScreen.y + h + 1 + 1), ImColor(255, 255, 255, 255), 30.f, ImVec2(0, 0));
				g_pBackgroundDrawList->AddShadowRect(ImVec2(r_x + w + 1, calc_y), ImVec2(r_x + w + 1 + 1, calc_y + frame_w + 1), ImColor(255, 255, 255, 255), 30.f, ImVec2(0, 0));
			}

			 
			FillRGBA(r_x, vTopScreen.y, frame_ww, 1, colorShit);
			FillRGBA(r_x, vTopScreen.y, 1, frame_w, colorShit);

			FillRGBA(r_x, vTopScreen.y - 1, frame_ww, 1, colorShit1);
			FillRGBA(r_x - 1, vTopScreen.y, 1, frame_w, colorShit1);

			 
			FillRGBA(calc_x, vTopScreen.y, frame_ww, 1, colorShit);
			FillRGBA(r_x + w, vTopScreen.y, 1, frame_w, colorShit);

			FillRGBA(calc_x, vTopScreen.y - 1, frame_ww + 1, 1, colorShit1);
			FillRGBA(r_x + w + 1, vTopScreen.y, 1, frame_w, colorShit1);

			 
			FillRGBA(r_x, vTopScreen.y + h, frame_ww, 1, colorShit);
			FillRGBA(r_x, calc_y, 1, frame_w, colorShit);

			FillRGBA(r_x, vTopScreen.y + h + 1, frame_ww, 1, colorShit1);
			FillRGBA(r_x - 1, calc_y, 1, frame_w + 1, colorShit1);

			 
			FillRGBA(calc_x + 1, vTopScreen.y + h, frame_ww, 1, colorShit);
			FillRGBA(r_x + w, calc_y, 1, frame_w, colorShit);

			FillRGBA(calc_x + 1, vTopScreen.y + h + 1, frame_ww, 1, colorShit1);
			FillRGBA(r_x + w + 1, calc_y, 1, frame_w + 1, colorShit1);
		}

		if (visuals::esp_health)
			DrawHealthBox(pPawn, vScreen.x - w * 0.5, vScreen.y + 6, w);

		if (visuals::esp_name)
		{
			const char* name = pPlayerController->m_sSanitizedPlayerName();
			std::string szName = name ? name : "";
			if (szName.size( ) > 0)
			{
				const ImVec2 min = { objBBox.x, objBBox.y };
				const ImVec2 max = { objBBox.w, objBBox.h };
				const ImVec2 vecTextSize = visuals::pixel_font->CalcTextSizeA(12, FLT_MAX, -1.0f, szName.c_str( ));
				ImVec2 textPos = ImFloor({ (min.x + max.x - vecTextSize.x) / 2.f + 2, min.y - 12.f });

				textPos.y -= 3.f;

				if (visuals::esp_glow)
					g_pBackgroundDrawList->AddShadowRect(textPos, textPos + vecTextSize, ImColor(255, 255, 255, 185), 60.f, ImVec2(0, 0));

				render::text_outline(g_pBackgroundDrawList, textPos - ImVec2(1.f, 1.f), IM_COL32(0, 0, 0, 255), szName.c_str( ));
				render::text_outline(g_pBackgroundDrawList, textPos, IM_COL32(255, 255, 255, 255), szName.c_str( ));
			}
		}
	}
}

void visuals::CalculateBoundingBoxes( )
{
	if (!interfaces::pEngine->IsInGame( )) return;

	const std::lock_guard<std::mutex> guard{ g_cachedEntitiesMutex };

	for (auto& it : g_cachedEntities)
	{
		C_BaseEntity* pEntity = it.m_handle.Get( );
		if (!pEntity) continue;

		 
		CHandle hEntity = pEntity->GetRefEHandle( );
		if (hEntity != it.m_handle) continue;

		switch (it.m_type)
		{
			case CachedEntity_t::PLAYER_CONTROLLER:
			{
				C_CSPlayerPawn* pPlayerPawn = ((CCSPlayerController*) pEntity)
					->m_hPawn( )
					.Get<C_CSPlayerPawn>( );
				if (pPlayerPawn)
					it.m_draw = pPlayerPawn->GetBoundingBox(it.m_bbox, false);

			} break;
			case CachedEntity_t::BASE_WEAPON:
				it.m_draw = pEntity->GetBoundingBox(it.m_bbox, true);
				break;
		}
	}
}

void visuals::CacheCurrentEntities( )
{

	if (!interfaces::pEngine->IsInGame( )) return;

	CGameEntitySystem* pEntitySystem = CGameEntitySystem::GetInstance( );
	if (!pEntitySystem) return;

	int highestIndex = pEntitySystem->GetHighestEntityIndex( );
	for (int i = 1; i <= highestIndex; ++i)
	{
		C_BaseEntity* pEntity = pEntitySystem->GetBaseEntity(i);
		if (!pEntity) continue;

		OnAddEntity(pEntity, pEntity->GetRefEHandle( ));
	}
}

void visuals::OnAddEntity(CEntityInstance* pInst, CHandle handle)
{
	C_BaseEntity* pEntity = (C_BaseEntity*) pInst;
	if (!pEntity) return;

	if (!handle.IsValid() || handle.GetEntryIndex( ) >= 32768) return;
	const std::lock_guard<std::mutex> guard{ g_cachedEntitiesMutex };

	auto it = std::find_if(g_cachedEntities.begin( ), g_cachedEntities.end( ),
						   [handle](const CachedEntity_t& i)
	{
		return i.m_handle.GetEntryIndex( ) ==
			handle.GetEntryIndex( );
	});

	if (it == g_cachedEntities.end( ))
	{
		CachedEntity_t cachedEntity{};
		cachedEntity.m_handle = handle;
		cachedEntity.m_type = ::GetEntityType(pEntity);
		if (cachedEntity.m_type != CachedEntity_t::UNKNOWN)
			g_cachedEntities.emplace_back(cachedEntity);
	}
	else
	{
		it->m_handle = handle;
		it->m_type = ::GetEntityType(pEntity);
	}
}

void visuals::OnRemoveEntity(CEntityInstance* pInst, CHandle handle)
{
	const std::lock_guard<std::mutex> guard{ g_cachedEntitiesMutex };
	C_BaseEntity* pEntity = (C_BaseEntity*) pInst;
	if (!pEntity) return;

	auto it = std::find_if(
		g_cachedEntities.begin( ), g_cachedEntities.end( ),
		[handle](const CachedEntity_t& i)
	{
		return i.m_handle == handle;
	});
	if (it == g_cachedEntities.end( )) return;

	g_cachedEntities.erase(it);
}

static CachedEntity_t::Type GetEntityType(C_BaseEntity* pEntity)
{
	if (pEntity->IsBasePlayerController( ))
		return CachedEntity_t::PLAYER_CONTROLLER;
	else if (pEntity->IsBasePlayerWeapon( ))
		return CachedEntity_t::BASE_WEAPON;

	return CachedEntity_t::UNKNOWN;
}

static void RenderPlayerESP(CCSPlayerController* pPlayerController, const BBox_t& bBox)
{
	using namespace visuals;

	if (!pPlayerController->m_bPawnIsAlive( ))
		return;

	C_CSPlayerPawn* pPawn = pPlayerController->m_hPawn( ).Get<C_CSPlayerPawn>( );
	if (!pPawn)
		return;

	const bool isLocalPlayer = pPlayerController->m_bIsLocalPlayerController( );
	const bool isEnemy = pPawn->IsEnemyWithTeam(g_pLocalPlayerController->m_iTeamNum( ));

	if (g_pLocalPlayerController->m_iTeamNum( ) == pPawn->m_iTeamNum( ))
		return;

	const ImVec2 min = { bBox.x, bBox.y };
	const ImVec2 max = { bBox.w, bBox.h };

	if (esp_box)
	{
		render::corner_outline(g_pBackgroundDrawList, min.x, min.y, max.x,
							   max.y, ImColor(255, 255, 255),
							   ImColor(56, 57, 72));
	}

	if (esp_name)
	{
		const char* szName = pPlayerController->m_sSanitizedPlayerName( );
		if (szName && strlen(szName) > 0)
		{
			const ImVec2 textSize = ImGui::CalcTextSize(szName);
			const ImVec2 textPos = ImFloor(
				{ (min.x + max.x - textSize.x) / 2.f + 2, min.y - textSize.y - 2.f });

			render::text_outline(g_pBackgroundDrawList, textPos,
								 IM_COL32(255, 255, 255, 255),
								 szName);
		}
	}
	if (esp_health)
	{
		const int hp = std::min(pPawn->m_iHealth( ), 100);

		const ImVec2 barMin = ImVec2{ min.x, max.y + 5 };
		const ImVec2 barMax = ImVec2{ max.x, max.y + 5 };

		const float w = ((barMax.x - barMin.x) * hp) / 100.f;
	}

}

static void RenderWeaponESP(C_CSWeaponBase* pWeapon, const BBox_t& bBox)
{
	using namespace visuals;

	if (pWeapon->m_hOwnerEntity( ).IsValid( )) return;

	if (fWeaponMaxDistance != 0.f &&
		g_pLocalPlayerPawn->DistanceToSquared(pWeapon) >=
		fWeaponMaxDistance * fWeaponMaxDistance)
		return;

	const ImVec2 min = { bBox.x, bBox.y };
	const ImVec2 max = { bBox.w, bBox.h };

}

static void RenderWeaponName(C_CSWeaponBase* pWeapon, const BBox_t& bBox)
{
	 
	C_AttributeContainer* pAttributeContainer = pWeapon->m_AttributeManager( );
	if (!pAttributeContainer) return;

	C_EconItemView* pItemView = pAttributeContainer->m_Item( );
	if (!pItemView) return;

	CEconItemDefinition* pItemStaticData = pItemView->GetStaticData( );
	if (!pItemStaticData) return;

	const char* szWeaponName =
		interfaces::pLocalize->FindSafe(pItemStaticData->m_pszItemBaseName);
	if (!szWeaponName || strlen(szWeaponName) < 1) return;

	const ImVec2 min = { bBox.x, bBox.y };
	const ImVec2 max = { bBox.w, bBox.h };

	const ImVec2 textSize = ImGui::CalcTextSize(szWeaponName);
	const ImVec2 textPos = ImFloor(
		{ (min.x + max.x - textSize.x) / 2.f, max.y + textSize.y - 12.f });
	visuals::g_pBackgroundDrawList->AddText(textPos + ImVec2{ 1, 1 },
											IM_COL32(0, 0, 0, 255), szWeaponName);
	visuals::g_pBackgroundDrawList->AddText(textPos, IM_COL32(255, 255, 255, 255),
											szWeaponName);
}
