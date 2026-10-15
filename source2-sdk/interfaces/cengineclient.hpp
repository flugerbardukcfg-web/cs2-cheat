#pragma once

#include "../../virtual.hpp"

class CEngineClient {
   public:
    auto IsInGame() { return CALL_VIRTUAL(bool, 39U, this); }
    auto IsConnected() { return CALL_VIRTUAL(bool, 40U, this); }
};
