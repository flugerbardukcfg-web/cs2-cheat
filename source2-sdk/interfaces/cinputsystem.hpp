#pragma once

class CInputSystem
{
public:
	bool IsRelativeMouseMode() { return CALL_VIRTUAL(bool, 77, this); }

	bool vcall_IsRelativeMouseMode( )
	{
		return reinterpret_cast<bool(__thiscall*)(void*)>(vmt::GetVMethod(77, this))(this);
	};

	void* GetSDLWindow( )
	{
		 
		 
		return *reinterpret_cast<void**>((uintptr_t) (this) + 0x2648);
	}
};
