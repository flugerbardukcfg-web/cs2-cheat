#pragma once

#include "c_csplayerpawnbase.hpp"
#include "../../../sdk/interfaces/interfaces.hpp"
#include "../../../sdk/source2-sdk/interfaces/cgameresourceserviceclient.hpp"

class EntitySpottedState_t
{
public:
	SCHEMA_FIELD(m_bSpotted, "EntitySpottedState_t", "m_bSpotted", bool);
	SCHEMA_FIELD(m_bSpottedByMask, "EntitySpottedState_t", "m_bSpottedByMask", int);
};

class C_CSWeaponBase;

class C_CSPlayerPawn : public C_CSPlayerPawnBase
{
public:
 SCHEMA_FIELD(m_vecViewOffset, "C_BaseModelEntity", "m_vecViewOffset", Vector);
	bool IsEnemyWithTeam(int team);
	bool IsVisible(C_CSPlayerPawn* pPawn, Vector vecPoint, bool bSmokeCheck, float flMaxDensity);

	SCHEMA_FIELD(GetShotsFired, "C_CSPlayerPawn", "m_iShotsFired", int32_t);
	SCHEMA_FIELD(m_pAimPunchServices, "C_CSPlayerPawn", "m_pAimPunchServices", void*);
 QAngle_t GetAimPuchAngle() {
  using fn_t = void(__fastcall*)(void*,QAngle_t*,bool);
  static auto fn = CModule(CLIENT_DLL).FindPattern(SDK_SIG("E8 ? ? ? ? 4C 8B C0 48 8D 55 ? 48 8B CB E8 ? ? ? ? 48 8D 0D")).ToAbsolute(1, 0).Get<fn_t>();
  QAngle_t result{}; if (fn && m_pAimPunchServices()) fn(this,&result,true); return result;
 }
	CUtlVector<QAngle_t>& GetAimPuchAngleCache() {
 static thread_local QAngle_t angle{}; static thread_local CUtlVector<QAngle_t> cache{};
 angle=GetAimPuchAngle(); cache.m_size=1; cache.m_data=&angle; return cache;
 }
	SCHEMA_FIELD(getViewAngles, "C_BasePlayerPawn", "v_angle", QAngle_t);
	PSCHEMA_FIELD(m_entitySpottedState, "C_CSPlayerPawn", "m_entitySpottedState", EntitySpottedState_t);

	int getEntitySpotted() {
		auto datatable_hash = hash_32_fnv1a_const("C_CSPlayerPawn");
		auto prop_hash = hash_32_fnv1a_const("m_entitySpottedState");
		auto offset = schema::GetOffset("C_CSPlayerPawn", datatable_hash, "m_entitySpottedState", prop_hash);

		auto datatable_hash2 = hash_32_fnv1a_const("EntitySpottedState_t");
		auto prop_hash2 = hash_32_fnv1a_const("m_bSpottedByMask");
		auto offset2 = schema::GetOffset("EntitySpottedState_t", datatable_hash2, "m_bSpottedByMask", prop_hash2);

		 
		 
		auto spotted = m_entitySpottedState()->m_bSpottedByMask();
		return spotted;
	}

	[[nodiscard]] Vector GetEyePosition()
	{
		Vector vecEyePosition = Vector(0.0f, 0.0f, 0.0f);
		auto* node = m_pGameSceneNode();
        if (node) vecEyePosition = node->m_vecAbsOrigin() + m_vecViewOffset();
		return vecEyePosition;
	}

	inline uint32_t GetOwnerHandleIndex()
	{
		uint32_t Result = -1;
		if (!(GetCollision().GetSolidFlags() & 4))
			Result = this->m_hOwnerEntity().GetEntryIndex();

		return Result;
	}

	inline uint16_t GetCollisionMask()
	{
		return GetCollision().CollisionMask();  
	}

	C_CSWeaponBase* GetWeaponActive();
};

class CGlowProperty
{
private:
	char pad_0000[0x18];

public:
	CEntityInstance* m_hOwner;

	SCHEMA_FIELD(GlowColorOverride, "CGlowProperty", "m_glowColorOverride", Color_t);
	SCHEMA_FIELD(Glowing, "CGlowProperty", "m_bGlowing", bool);
};