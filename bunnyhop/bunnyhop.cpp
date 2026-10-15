#include "bunnyhop.h"
#include "../../sdk/interfaces/interfaces.hpp"
#include "../../sdk/source2-sdk/types/chandle.hpp"
bool bWasLastTimeOnGround;
static auto last_jumped = false;
static auto should_jump = false;
#define CheckIfNonValidNumber(x)                                \
    (fpclassify(x) == FP_INFINITE || fpclassify(x) == FP_NAN || \
     fpclassify(x) == FP_SUBNORMAL)
float normalize_yaw(float f)
{
	while (f < -180.0f) f += 360.0f;

	while (f > 180.0f) f -= 360.0f;

	return f;
}
float get_delta(float hspeed, float maxspeed, float airaccelerate)
{
	auto term = (30.0 - (airaccelerate * maxspeed / 66.0)) / hspeed;

	if (term < 1.0f && term > -1.0f)
	{
		return acos(term);
	}

	return 0.f;
}
void bunnyhop::run(CUserCmd* cmd, QAngle_t* viewangle)
{
	if (!bBunnyHop) return;
	static float old_yaw{};
	CCSPlayerController* lcontroller =
		CGameEntitySystem::GetLocalPlayerController( );
	if (!lcontroller) return;
	C_CSPlayerPawn* localplayer = lcontroller->m_hPawn( ).Get<C_CSPlayerPawn>( );
	if (!localplayer || localplayer->m_iHealth( ) <= 0) return;

	constexpr MoveType bad_mts[ ] = { movetype_observer, movetype_noclip,
									movetype_observer, movetype_ladder };
	const auto bad_mt = std::ranges::find(bad_mts, localplayer->m_MoveType( )) !=
		std::end(bad_mts);
	if (bad_mt) return;
	if (localplayer->m_fFlags( ).has_flag(fl_onground))
	{
		old_yaw = 0.f;
		return;
	}

	Vector velocity = localplayer->m_vecVelocity( );
	velocity.z = 0.f;

	auto forwardmove = cmd->pBase.pBase->flForwardMove;
	auto sidemove = cmd->pBase.pBase->flSideMove;

	if (velocity.Length_2D( ) < 500.0f && !forwardmove && !sidemove) return;

	Vector view_angle = { viewangle->x, viewangle->y, viewangle->z };

	int button = cmd->buttonStates.nButtonState1;
	float yaw_add = 01.f;
	bool back = button & IN_BACK;
	bool forward = button & IN_FORWARD;
	bool right = button & IN_MOVELEFT;
	bool left = button & IN_MOVERIGHT;

	if (back)
	{
		yaw_add = -1 / 2;
		if (right)
			yaw_add -= 1;
		else if (left)
			yaw_add += 1;
	}
	else if (right)
	{
		yaw_add = 1 / 2;
		if (back)
			yaw_add += 1;
		else if (forward)
			yaw_add -= 1;
	}
	else if (left)
	{
		yaw_add = -1 / 2;
		if (back)
			yaw_add -= 1;
		else if (forward)
			yaw_add += 1;
	}
	else
	{
		yaw_add = 1.f;
	}

	view_angle.y += yaw_add;

	cmd->pBase.pBase->flForwardMove = 0;
	cmd->pBase.pBase->flSideMove = 0;

	auto yaw = math::normalize_yaw(
		view_angle.y - M_DEG2RAD(atan2(localplayer->m_vecVelocity( ).y,
		localplayer->m_vecVelocity( ).x)));

	if (yaw < 0.f) cmd->pBase.pBase->flSideMove = -1;
	if (yaw > 0.f) cmd->pBase.pBase->flSideMove = 1;

	(*viewangle).y = math::normalize_yaw(view_angle.y - yaw);


}
