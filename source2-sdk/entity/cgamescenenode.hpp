#pragma once

#include "../../math/types/vector.hpp"
#include "../../schema/schema.hpp"
#include "../../virtual.hpp"

class CSkeletonInstance;
class CGameSceneNode
{
   public:
    void SetMeshGroupMask(uint64_t meshGroupMask);
    SCHEMA_FIELD(m_vecAbsOrigin, "CGameSceneNode", "m_vecAbsOrigin", Vector);

	uintptr_t getBoneArray() {
		static constexpr auto datatable_hash = hash_32_fnv1a_const("CSkeletonInstance");
		static constexpr auto prop_hash = hash_32_fnv1a_const("m_modelState");
		static auto offset = schema::GetOffset("CSkeletonInstance", datatable_hash, "m_modelState", prop_hash);
		auto boneArray = *reinterpret_cast<uintptr_t*>(reinterpret_cast<uint8_t*>(this) + offset + 0x80);
		return boneArray;
	}

	auto GetSkeletonInstance()
	{
		return CALL_VIRTUAL(CSkeletonInstance*, 8U, this);
	}

};

struct Matrix2x4_t
{
public:
	[[nodiscard]] const Vector GetOrigin(int nIndex)
	{
		return Vector(this[nIndex]._11, this[nIndex]._12, this[nIndex]._13);
	}

	const void SetOrigin(int nIndex, Vector vecValue)
	{
		this[nIndex]._11 = vecValue.x;
		this[nIndex]._12 = vecValue.y;
		this[nIndex]._13 = vecValue.z;
	}

	[[nodiscard]] const Vector4D GetRotation(int nIndex)
	{
		return Vector4D(this[nIndex]._21, this[nIndex]._22, this[nIndex]._23, this[nIndex]._24);
	}

	union
	{
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
		};
	};
};

class CModelState {
public:
    SCHEMA_FIELD(m_MeshGroupMask, "CModelState", "m_MeshGroupMask", uint64_t);
};
struct CSkeletonInstance {
    SCHEMA_FIELD(m_nHitboxSet, "CSkeletonInstance", "m_nHitboxSet", uint8_t);
    SCHEMA_FIELD(m_modelState, "CSkeletonInstance", "m_modelState", CModelState);
};
