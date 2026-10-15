#pragma once
#include "../../utils/external/config.hpp"
#include "../../sdk/math/math.hpp"
#include <windows.h>
#include <map>
#include <vector>
#include <string>

struct moduleData {
	HMODULE module;
	DWORD_PTR base;
	uintptr_t size;
};

class C_CSPlayerPawn;
class CUserCmd;
class CBaseUserCmdPB;

namespace aim {
	Vector recoilControl(bool move, C_CSPlayerPawn* localPawn);
	void aimBot(C_CSPlayerPawn* enemyPlayer, uintptr_t boneArray, CUserCmd* pCmd, CBaseUserCmdPB* pBaseCmd);
	void moveMouseToLocation(Vector pos);
	 

    inline C_CSPlayerPawn* lockedPlayer = 0;

    inline auto& isHotTrigger = aimConf.isHotTrigger;
    inline auto& hotSelectTrigger = aimConf.hotSelectTrigger;
    inline auto& hotTrigger = aimConf.hotTrigger;
    inline auto& playerLock = aimConf.playerLock;

    inline auto& state = aimConf.state;
    inline auto& rcs = aimConf.rcs;
    inline auto& trigger = aimConf.trigger;

    inline auto& alaimlock = aimConf.alaimlock;
    inline auto& checkSpotted = aimConf.checkSpotted;
    inline auto& smoothing = aimConf.smoothing;

    inline auto& fov = aimConf.fov;
    inline auto& fovCircle = aimConf.fovCircle;

    inline auto& bone = aimConf.bone;
    inline auto& boneSelect = aimConf.boneSelect;
    inline auto& bones = aimConf.bones;
    inline auto& boneMap = aimConf.boneMap;

    inline auto& aimMode = aimConf.aimMode;
    inline auto& aimModes = aimConf.aimModes;
    inline auto& aimModeMap = aimConf.aimModeMap;

    inline auto& hotSelectAim = aimConf.hotSelectAim;
    inline auto& hotAim = aimConf.hotAim;

    inline float sens = 2.25f;
}
