#include "memewalk.h"

#include "../valve_sdk/csgostructs.hpp"


void memewalk::OnCreateMove(CUserCmd* cmd)
{
	auto pLocal = (C_BasePlayer*)g_EntityList->GetClientEntity(g_EngineClient->GetLocalPlayer());

	if (pLocal->GetMoveType() == MOVETYPE_LADDER)
	return;

	if (cmd->forwardmove > 0)
	{
		cmd->buttons |= IN_BACK;
		cmd->buttons &= ~IN_FORWARD;
	}

	if (cmd->forwardmove < 0)
	{
		cmd->buttons |= IN_FORWARD;
		cmd->buttons &= ~IN_BACK;
	}

	if (cmd->sidemove < 0)
	{
		cmd->buttons |= IN_MOVERIGHT;
		cmd->buttons &= ~IN_MOVELEFT;
	}

	if (cmd->sidemove > 0)
	{
		cmd->buttons |= IN_MOVELEFT;
		cmd->buttons &= ~IN_MOVERIGHT;
	}
}



































































































