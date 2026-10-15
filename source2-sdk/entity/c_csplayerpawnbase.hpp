#pragma once

#include "ccsplayer_viewmodelservices.hpp"
#include "c_baseplayerpawn.hpp"

class C_CSPlayerPawnBase : public C_BasePlayerPawn {
   public:
    SCHEMA_FIELD(m_flFlashMaxAlpha, "C_CSPlayerPawnBase", "m_flFlashMaxAlpha", float);
    SCHEMA_FIELD(m_pViewModelServices, "C_CSPlayerPawnBase",
                 "m_pViewModelServices", CCSPlayer_ViewModelServices*);
    SCHEMA_FIELD(m_iIDEntIndex, "C_CSPlayerPawn",
        "m_iIDEntIndex", int);
};
