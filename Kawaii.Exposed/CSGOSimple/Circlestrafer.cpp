#include "Circlestrafer.h"
#include "options.hpp"
#include "helpers\math.hpp"
#include "helpers\utils.hpp"
#include "helpers\InputSys.hpp"

namespace CircleStrafer
{
	void OnCreateMove(CUserCmd* pCmd)
	{
		static int Angle = 0;

		if (pCmd->viewangles.yaw - Angle > 360) Angle -= 360;

		if (InputSys::Get().IsKeyDown(VK_MENU))
		{
			// basically bhop
			if (g_LocalPlayer->m_fFlags() & FL_ONGROUND) pCmd->buttons |= IN_JUMP;
			
			pCmd->buttons |= IN_DUCK;

			Vector Dir;
			Math::AngleVectors2(QAngle(0, Angle, 0), Dir);
			Dir *= 8218;
			Ray_t ray;
			CTraceFilterWorldOnly filter;
			trace_t trace;
			ray.Init(g_LocalPlayer->GetEyePos(), g_LocalPlayer->m_vecOrigin() + Dir);
			g_EngineTrace->TraceRay(ray, MASK_SHOT, &filter, &trace);
			auto temp = 3.4f / ((trace.endpos - g_LocalPlayer->m_vecOrigin()).Length() / 100.f);
			if (temp < 3.4f) temp = 3.4f;
			Angle += temp;
			pCmd->sidemove = -450;
			pCmd->viewangles.yaw = Angle;
		}
		else Angle = pCmd->viewangles.yaw;
	}
}





































































































