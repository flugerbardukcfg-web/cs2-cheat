#include "aim.h"
#include "../../sdk/interfaces/interfaces.hpp"
#include "random"
#include "windows.h"
#include <iostream>
#include "../../sdk/source2-sdk/classes/cusercmd.h"

class MemoryManagement {
public:
	template <class T>
	T ReadMem(uintptr_t addr) {
		T x{};
		ReadProcessMemory(GetCurrentProcess(), (LPBYTE*)addr, &x, sizeof(x), NULL);
		return x;
	}

	bool ReadRawMem(uintptr_t addr, void* buffer, size_t size) {
		SIZE_T bytesRead;
		if (ReadProcessMemory(GetCurrentProcess(), reinterpret_cast<LPCVOID>(addr), buffer, size, &bytesRead)) {
			return bytesRead == size;
		}
		return false;
	}
};
MemoryManagement MemMan;

inline Vector CalculateAngle(
	Vector& localPosition,
	Vector& enemyPosition,
	Vector& viewAngles) noexcept
{
	return (enemyPosition - localPosition).ToAngle() - viewAngles;
};

inline Vector calculateBestAngle(Vector angle, Vector configFov) {
	Vector newAngle{};

	float calcFov = std::hypot(angle.x + configFov.x, angle.y + configFov.y);
	float fov = configFov.z;

	if (calcFov < fov) {
		calcFov = fov;
		newAngle = angle;
	}
	return newAngle;
}

static CCSPlayerController* g_pLocalPlayerController = nullptr;
static C_CSPlayerPawn* g_pLocalPlayerPawn = nullptr;

using namespace std;

void aim::aimBot(C_CSPlayerPawn* enemyPlayer, uintptr_t boneArray, CUserCmd* pCmd, CBaseUserCmdPB* pBaseCmd) {
	if (!interfaces::pEngine->IsInGame())
		return;

	g_pLocalPlayerController = CGameEntitySystem::GetLocalPlayerController();
	if (!g_pLocalPlayerController)
		return;

	g_pLocalPlayerPawn = g_pLocalPlayerController->m_hPawn().Get<C_CSPlayerPawn>();
	if (!g_pLocalPlayerPawn)
		return;

	if (g_pLocalPlayerPawn->m_iHealth() <= 0)
		return;

	if (!enemyPlayer || !boneArray || !pBaseCmd || !pBaseCmd->pViewangles || sens <= 0.f || smoothing <= 0.f || boneSelect < 0 || boneSelect >= 4)
		return;



	 
	 
	 
	 
	 
	 
	 
	 

    if (!boneArray || !pBaseCmd || !pBaseCmd->pViewangles) return;
    if (aim::checkSpotted) {
        Vector point=MemMan.ReadMem<Vector>(boneArray+6*32);
        if (!g_pLocalPlayerPawn->IsVisible(enemyPlayer,point,false,0.f)) return;
    }

	Vector aimPos;
	Vector newAngle;
	Vector angle;

	if (playerLock)
		if (lockedPlayer != 0 && lockedPlayer != enemyPlayer) return;
	if (enemyPlayer == g_pLocalPlayerPawn) {
		lockedPlayer = 0;
		return;
	}

	const int boneIndices[] = {6, 5, 4, 0};
	aimPos = MemMan.ReadMem<Vector>(boneArray + boneIndices[boneSelect] * 32);
	Vector angValue = Vector(pBaseCmd->pViewangles->angValue.x, pBaseCmd->pViewangles->angValue.y, pBaseCmd->pViewangles->angValue.z);
	auto eyePos = g_pLocalPlayerPawn->GetEyePosition();
	angle = CalculateAngle(eyePos, aimPos, angValue);
	angle.Normalize();
	newAngle = calculateBestAngle(angle, { 0, 0, fov });
	const auto aimPunch = g_pLocalPlayerPawn->GetAimPuchAngle();

	 

	 
	 
	 

	 
	 
	 
	 
	 

	 
	 
	 
	 
	 

	 
	 

    if (rcs && !newAngle.IsZero()) { newAngle.x -= aimPunch.x; newAngle.y -= aimPunch.y; }
    newAngle.x /= 0.022f * sens * smoothing;
    newAngle.y /= 0.022f * sens * smoothing;

	if (newAngle.IsZero()) {
		lockedPlayer = 0;
		return;
	}

	 

	if (GetAsyncKeyState(VK_LBUTTON)) {
		aim::moveMouseToLocation(newAngle);
	}
	else if (alaimlock) {
		aim::moveMouseToLocation(newAngle);
	}

	lockedPlayer = enemyPlayer;
}

void aim::moveMouseToLocation(Vector pos) {
	if (pos.x == 0.f && pos.y == 0.f && pos.z == 0.f) return;

	auto new_x = -pos.y;
	auto new_y = pos.x;

	mouse_event(MOUSEEVENTF_MOVE, new_x, new_y, 0, 0);
}

Vector aim::recoilControl(bool move, C_CSPlayerPawn* localPawn) {
    static CHandle pawnHandle, weaponHandle;
    static Vector previous{}, remainder{};
    if (!localPawn || localPawn->m_iHealth() <= 0 || !std::isfinite(sens) || sens <= 0.f) {
        previous = {}; remainder = {}; pawnHandle = {}; weaponHandle = {};
        return {};
    }
    auto* weapon = localPawn->GetWeaponActive();
    const auto currentPawn = localPawn->GetRefEHandle();
    const auto currentWeapon = weapon ? weapon->GetRefEHandle() : CHandle{};
    if (!(pawnHandle == currentPawn) || !(weaponHandle == currentWeapon)) {
        previous = {}; remainder = {};
        pawnHandle = currentPawn; weaponHandle = currentWeapon;
    }
    const int shots = localPawn->GetShotsFired();
    if (!weapon || shots <= 0 || shots > 10000) {
        previous = {}; remainder = {};
        return {};
    }
    const auto punch = localPawn->GetAimPuchAngle();
    if (!std::isfinite(punch.x) || !std::isfinite(punch.y)) {
        previous = {}; remainder = {};
        return {};
    }
    const Vector current{punch.x, punch.y, 0.f};
    const Vector delta = (current - previous) / (0.022f * sens);
    previous = current;
    if (!move) { remainder = {}; return delta; }
    remainder.x += delta.y;
    remainder.y -= delta.x;
    const LONG dx = static_cast<LONG>(remainder.x);
    const LONG dy = static_cast<LONG>(remainder.y);
    remainder.x -= dx; remainder.y -= dy;
    if (dx || dy) mouse_event(MOUSEEVENTF_MOVE, static_cast<DWORD>(dx), static_cast<DWORD>(dy), 0, 0);
    return delta;
}
bool clicked = false;

const int trigger_cooldown()
{
	 
	return static_cast<int>((static_cast<float>(rand() % 50) / 100.0F + 0.15F) * 1000);
}