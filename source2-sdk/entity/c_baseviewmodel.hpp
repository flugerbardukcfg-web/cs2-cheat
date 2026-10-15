#pragma once

#include "c_basemodelentity.hpp"

class C_BaseViewModel : public C_BaseModelEntity {
   public:
    SCHEMA_FIELD(m_hWeapon, "C_BaseEntity", "m_hOwnerEntity", CHandle);
};

class CAnimGraphNetworkedVariables;
class CAnimationGraphInstance2
{
public:
	char pad_0x0000[0x2E0];  
	CAnimGraphNetworkedVariables* pAnimGraphNetworkedVariables;  
};

class C_CSGOViewModel2 : public C_BaseViewModel
{
public:
	char pad_0x0000[0xD60];  
	CAnimationGraphInstance2* pAnimationGraphInstance;  
};
