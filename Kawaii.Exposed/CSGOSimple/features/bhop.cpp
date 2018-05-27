#include "bhop.hpp"

#include "../valve_sdk/csgostructs.hpp"
#include "../helpers/InputSys.hpp"
#include "../Options.hpp"


// meme bhop code lol

//void BunnyHop::OnCreateMove(CUserCmd* cmd)
//{
//	static bool jumped_last_tick = false;
//	static bool should_fake_jump = false;
//
//	if (!jumped_last_tick && should_fake_jump)
//	{
//		should_fake_jump = false;
//		cmd->buttons |= IN_JUMP;
//	}
//	else if (IN_JUMP && GetKeyState(VK_SPACE) && g_Options.misc_bhop_key == true)
//	{
//		if (g_LocalPlayer->m_fFlags() & FL_ONGROUND)
//		{
//			jumped_last_tick = true;
//			should_fake_jump = true;
//		}
//		else
//		{
//			cmd->buttons &= ~IN_JUMP;
//			jumped_last_tick = false;
//		}
//	}
//	else
//	{
//		jumped_last_tick = false;
//		should_fake_jump = false;
//	}
//}


// ayyware bhop

void BunnyHop::OnCreateMove(CUserCmd *pCmd)
{
	if (InputSys::Get().IsKeyDown(VK_SPACE) && g_Options.misc_bhop_key)
	{
		int iFlags = g_LocalPlayer->m_fFlags();
		if (!(iFlags & FL_ONGROUND))
			pCmd->buttons &= ~IN_JUMP;

		if (g_LocalPlayer->m_vecVelocity().Length() <= 50)
		{
			pCmd->forwardmove = 450.f;
		}
	}
}


































































































