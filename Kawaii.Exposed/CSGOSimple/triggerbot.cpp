#include "triggerbot.h"
#include "helpers\math.hpp"
#include "options.hpp"
#include "hooks.hpp"

#define HITGROUP_GENERIC 0
#define HITGROUP_HEAD 1
#define HITGROUP_CHEST 2
#define HITGROUP_STOMACH 3
#define HITGROUP_LEFTARM 4
#define HITGROUP_RIGHTARM 5
#define HITGROUP_LEFTLEG 6
#define HITGROUP_RIGHTLEG 7

namespace triggerbot {

	void Triggerbot(CUserCmd* pCmd) {

			if (g_LocalPlayer->IsAlive() && g_LocalPlayer->IsAlive() && !(g_LocalPlayer->m_lifeState() & LIFE_DYING))
			{
				Vector src, dst, forward;
				trace_t tr;
				Ray_t ray;
				CTraceFilter filter;

				QAngle viewangle = pCmd->viewangles;

				viewangle += g_LocalPlayer->m_aimPunchAngle() * 2.f;

				Math::AngleVectors2(viewangle, forward);

				forward *= g_LocalPlayer->m_hActiveWeapon()->GetCSWeaponData()->flRange;
				filter.pSkip = g_LocalPlayer;
				src = g_LocalPlayer->GetEyePos();
				dst = src + forward;
				ray.Init(src, dst);

				g_EngineTrace->TraceRay(ray, 0x46004003, &filter, &tr);

				if (!tr.hit_entity)
					return;

				int hitgroup = tr.hitgroup;
				bool didHit = false;
				if ((g_Options.triggerbot_spot == tbHead) && hitgroup == HITGROUP_HEAD)
					didHit = true;
				if ((g_Options.triggerbot_spot == tbChest) && hitgroup == HITGROUP_CHEST)
					didHit = true;
				if ((g_Options.triggerbot_spot == tbStomach) && hitgroup == HITGROUP_STOMACH)
					didHit = true;
				if ((g_Options.triggerbot_spot == tbAll) && hitgroup == HITGROUP_HEAD || hitgroup == HITGROUP_CHEST || hitgroup == HITGROUP_STOMACH || hitgroup == HITGROUP_LEFTARM || hitgroup == HITGROUP_LEFTLEG || hitgroup == HITGROUP_RIGHTARM || hitgroup == HITGROUP_LEFTLEG)
					didHit = true;

				if (didHit && (tr.hit_entity->GetBaseEntity()->m_iTeamNum() != g_LocalPlayer->m_iTeamNum()))
				{
					if (g_Options.triggerbotkey > 0 && GetAsyncKeyState(g_Options.triggerbotkey) & 0x8000 && g_Options.triggerbotactive)
					{
						pCmd->buttons |= IN_ATTACK;
					}
				}
			}
		}
	}


































































































