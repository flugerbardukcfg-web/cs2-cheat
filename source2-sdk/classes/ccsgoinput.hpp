#pragma once
#include <stb/stb.hh>
#include "cusercmd.h"
#define MULTIPLAYER_BACKUP 150

#define _CS_INTERNAL_CONCATENATE(LEFT, RIGHT) LEFT##RIGHT
#define CS_CONCATENATE(LEFT, RIGHT) _CS_INTERNAL_CONCATENATE(LEFT, RIGHT)

#define MEM_PAD(SIZE)										\
private:												\
	char CS_CONCATENATE(pad_0, __COUNTER__)[SIZE]; \
public:

class CTinyMoveStepData
{
public:
	float flWhen;  
	MEM_PAD(0x4);  
	std::uint64_t nButton;  
	bool bPressed;  
	MEM_PAD(0x7);  
};  

class CMoveStepButtons
{
public:
	std::uint64_t nKeyboardPressed;  
	std::uint64_t nMouseWheelheelPressed;  
	std::uint64_t nUnPressed;  
	std::uint64_t nKeyboardCopy;  
};  

 
class CExtendedMoveData : public CMoveStepButtons
{
public:
	float flForwardMove;  
	float flSideMove;  
	float flUpMove;  
	std::int32_t nMouseDeltaX;  
	std::int32_t nMouseDeltaY;  
	std::int32_t nAdditionalStepMovesCount;  
	CTinyMoveStepData tinyMoveStepData[12];  
	Vector vecViewAngle;  
	std::int32_t nTargetHandle;  
};  

class CCSGOInput 
{
 public:
	 MEM_PAD(0x250);
	 CUserCmd arrCommands[MULTIPLAYER_BACKUP];
	 MEM_PAD(0x99)
	 bool bInThirdPerson;
	 MEM_PAD(0x6);
	 QAngle_t angThirdPersonAngles;
	 MEM_PAD(0xE);
	 std::int32_t nSequenceNumber;
	 double dbSomeTimer;
	 CExtendedMoveData currentMoveData;
	 std::int32_t nWeaponSwitchTick;
	 MEM_PAD(0x1C4);
	 CExtendedMoveData* pExtendedMoveData;
	 MEM_PAD(0x48);
	 int32_t nAttackStartHistoryIndex1;
	 int32_t nAttackStartHistoryIndex2;
	 int32_t nAttackStartHistoryIndex3;

    CUserCmd* GetUserCmd();

	QAngle_t* GetViewAngles() {
		if (!memory::m_CSGOInput_GetViewAngles) return nullptr;
        return reinterpret_cast<QAngle_t*(__fastcall*)(CCSGOInput*, int32_t)>(memory::m_CSGOInput_GetViewAngles)(this, 0);
	}

	bool SetViewAngles(QAngle_t& vecAngles) {
		if (!memory::m_CSGOInput_SetViewAngles) return false;
        reinterpret_cast<void(__fastcall*)(CCSGOInput*, int32_t, const QAngle_t*)>(memory::m_CSGOInput_SetViewAngles)(this, 0, &vecAngles);
        return true;
	}
};
