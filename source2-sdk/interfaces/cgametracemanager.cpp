#include <stb/stb.hh>
 
#include "cgametracemanager.h"
 
#include "../entity/c_baseentity.hpp"
#include "../entity/c_csplayerpawn.hpp"
#include "../../memory/memory.hpp"

SurfaceData_t* GameTrace_t::GetSurfaceData( )
{
	return memory::fnGetSurfaceData && m_pSurface ? reinterpret_cast<SurfaceData_t*>(memory::fnGetSurfaceData(m_pSurface)) : nullptr;
}

int GameTrace_t::GetHitboxId( )
{
	if (m_pHitboxData)
		return m_pHitboxData->m_nHitboxId;
	return 0;
}

int GameTrace_t::GetHitgroup( )
{
	if (m_pHitboxData)
		return m_pHitboxData->m_nHitGroup;
	return 0;
}

bool GameTrace_t::IsVisible( ) const
{
	return (m_flFraction > 0.97f);
}

TraceFilter_t::TraceFilter_t(uint64_t mask,C_CSPlayerPawn* first,C_CSPlayerPawn* second,int layer) : TraceFilter_t() {
 using fn_t = void(__fastcall*)(TraceFilter_t*,void*,uint64_t,int);
 static auto fn = CModule(CLIENT_DLL).FindPattern(SDK_SIG("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 0F B6 41 ? 33 FF 24")).Get<fn_t>();
 if (fn) fn(this,first ? first : second,mask,layer);
 m_v2=7;
}
