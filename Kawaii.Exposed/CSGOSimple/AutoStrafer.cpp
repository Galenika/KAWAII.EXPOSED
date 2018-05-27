#include "AutoStrafer.h"
#include "options.hpp"
#include "helpers\math.hpp"

namespace AutoStrafer
{
	void LegitStrafe(CUserCmd* pCmd)
	{
		if (g_LocalPlayer->m_fFlags() & FL_ONGROUND)
			return;

		if (pCmd->buttons & IN_FORWARD || pCmd->buttons & IN_BACK || pCmd->buttons & IN_MOVELEFT || pCmd->buttons & IN_MOVERIGHT)
			return;

		if (pCmd->mousedx <= 1 && pCmd->mousedx >= -1)
			return;

		pCmd->sidemove = pCmd->mousedx < 0.f ? -450.f : 450.f;
	}

	void RageStrafe(CUserCmd* pCmd)
	{
		static bool leftRight;
		bool inMove = pCmd->buttons & IN_FORWARD || pCmd->buttons & IN_BACK || pCmd->buttons & IN_MOVELEFT || pCmd->buttons & IN_MOVERIGHT;

		if (pCmd->buttons & IN_FORWARD && g_LocalPlayer->m_vecVelocity().Length() <= 50.0f)
			pCmd->forwardmove = 450.0f;

		float yaw_change = 0.0f;
		if (g_LocalPlayer->m_vecVelocity().Length() > 50.f)
			yaw_change = 30.0f * fabsf(30.0f / g_LocalPlayer->m_vecVelocity().Length());

		C_BaseCombatWeapon* activeWeapon = (C_BaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(g_LocalPlayer->m_hActiveWeapon());

		if (activeWeapon && !activeWeapon->GetCSWeaponData()->iBullets == 0 && pCmd->buttons & IN_ATTACK)
			yaw_change = 0.0f;

		QAngle viewAngles;
		g_EngineClient->GetViewAngles(viewAngles);

		if (!(g_LocalPlayer->m_fFlags() & FL_ONGROUND) && !inMove)
		{
			if (leftRight || pCmd->mousedx > 1)
			{
				viewAngles.yaw += yaw_change;
				pCmd->sidemove = 450.0f;
			}
			else if (!leftRight || pCmd->mousedx < 1)
			{
				viewAngles.yaw -= yaw_change;
				pCmd->sidemove = -450.0f;
			}

			leftRight = !leftRight;
		}

		Math::NormalizeAngles(viewAngles);
		Math::ClampAngles(viewAngles);

		pCmd->viewangles = viewAngles;
	}

	void OnCreateMove(CUserCmd* pCmd)
	{
		switch (g_Options.autostrafer)
		{
		case AUTOSTRAFER_NONE:
			break;

		case AUTOSTRAFER_LEGIT:
			LegitStrafe(pCmd);
			break;
		case AUTOSTRAFER_RAGE:
			RageStrafe(pCmd);
			break;
		}
	}
}





































































































