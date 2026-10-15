#include "movement.hpp"
#include <algorithm>
#include <cmath>

void movement::fix(CBaseUserCmdPB* command, QAngle_t previous)
{
    if (!command || !command->pViewangles) return;
    const float delta = (command->pViewangles->angValue.y - previous.y) * (std::numbers::pi_v<float> / 180.f);
    const float forward = command->flForwardMove;
    const float side = command->flSideMove;
    if (!std::isfinite(delta) || !std::isfinite(forward) || !std::isfinite(side)) return;
    command->flForwardMove = std::clamp(std::cos(delta) * forward - std::sin(delta) * side, -1.f, 1.f);
    command->flSideMove = std::clamp(std::sin(delta) * forward + std::cos(delta) * side, -1.f, 1.f);
}
