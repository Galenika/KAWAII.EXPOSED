#include "thirdperson.h"
#include "GrenadePrediction.h"
#include "../Helpers/math.hpp"
#include "../Options.hpp"

//namespace grenadeprediction
//{
//	FORCEINLINE vec_t DotProduct(const Vector& a, const Vector& b)
//	{
//		return (a.x * b.x + a.y * b.y + a.z * b.z);
//	}
//
//	void PredictGrenade()
//	{
//		auto pWeapon = (C_BaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(g_LocalPlayer->m_hActiveWeapon());
//		const float TIMEALIVE = 5.f;
//		const float GRENADE_COEFFICIENT_OF_RESTITUTION = 0.4f;
//
//		float fStep = 0.1f;
//		float fGravity = 800.0f / 8.f;
//
//		Vector vPos;
//		Vector vStart;
//		QAngle vThrow;
//		Vector vThrow2;
//		int iCollisions = 0;
//
//		QAngle vViewAngles;
//		g_EngineClient->GetViewAngles(vViewAngles);
//
//		vThrow.pitch = vViewAngles.pitch;
//		vThrow.yaw = vViewAngles.yaw;
//		vThrow.roll = vViewAngles.roll;
//
//		if (vThrow.pitch < 0)
//			vThrow.pitch = -10 + vThrow.pitch * ((90 - 10) / 90.0);
//		else
//			vThrow.pitch = -10 + vThrow.pitch * ((90 + 10) / 90.0);
//
//		float fVel = (90 - vThrow.pitch) * 4;
//		if (fVel > 500)
//			fVel = 500;
//
//		Math::AngleVectors2(vThrow, vThrow2);
//
//		Vector vEye = g_LocalPlayer->GetEyePos();
//		vStart.x = vEye.x + vThrow2.x * 16;
//		vStart.y = vEye.y + vThrow2.y * 16;
//		vStart.z = vEye.z + vThrow2.z * 16;
//
//		vThrow2.x = (vThrow2.x * fVel) + g_LocalPlayer->m_vecVelocity().x;
//		vThrow2.y = (vThrow2.y * fVel) + g_LocalPlayer->m_vecVelocity().y;
//		vThrow2.z = (vThrow2.z * fVel) + g_LocalPlayer->m_vecVelocity().z; // casualhacker for da magic calc help
//
//		for (float fTime = 0.0f; fTime < TIMEALIVE; fTime += fStep)
//		{
//			vPos = vStart + vThrow2 * fStep;
//
//			Ray_t ray;
//			trace_t tr;
//
//			CTraceFilter loc;
//			loc.pSkip = g_LocalPlayer;
//
//			ray.Init(vStart, vPos);
//			g_EngineTrace->TraceRay(ray, MASK_SOLID, &loc, &tr);
//
//			if (tr.DidHit()) // if the trace hits a surface
//			{
//				vThrow2 = tr.plane.normal * -2.0f * DotProduct(vThrow2, tr.plane.normal) + vThrow2;
//				vThrow2 *= GRENADE_COEFFICIENT_OF_RESTITUTION;
//
//				iCollisions++;
//
//				if (iCollisions > 2)
//					break;
//				
//				vPos = vStart + vThrow2*tr.fraction*fStep;
//
//				fTime += (fStep * (1 - tr.fraction));
//			} // end of bounce
//
//			Vector vOutStart, vOutEnd;
//
//			if (pWeapon)
//			{
//				if (pWeapon->GetCSWeaponData()->WeaponType == WEAPONTYPE_GRENADE)
//				{
//					if (Math::WorldToScreen(vStart, vOutStart), Math::WorldToScreen(vPos, vOutEnd))
//					{
//						g_VGuiSurface->DrawSetColor(Color(255, 255, 255, 255));
//						g_VGuiSurface->DrawLine(vOutStart.x, vOutStart.y, vOutEnd.x, vOutEnd.y);
//					}
//
//					vStart = vPos;
//					vThrow2.z -= fGravity * tr.fraction * fStep;
//				}
//			}
//		}
//	}
//}

void grenade_prediction::Tick(int buttons)
{
	if (!g_Options.grenade_path_esp)
		return;
	bool in_attack = buttons & IN_ATTACK;
	bool in_attack2 = buttons & IN_ATTACK2;

	act = (in_attack && in_attack2) ? ACT_LOB :
		(in_attack2) ? ACT_DROP :
		(in_attack) ? ACT_THROW :
		ACT_NONE;
}
void grenade_prediction::View(CViewSetup* setup)
{

	auto local = g_LocalPlayer;
	if (!g_Options.grenade_path_esp)
		return;
	if (local && g_LocalPlayer->IsAlive())
	{

			auto pWeapon = local->m_hActiveWeapon();
			if (pWeapon) {
			if (pWeapon && pWeapon->IsGrenade() && act != ACT_NONE)
			{
				type = pWeapon->m_Item().m_iItemDefinitionIndex();
				Simulate(setup);
			}
			else
			{
				type = 0;
			}
		}
	}
}

void grenade_prediction::Paint()
{
	if (!g_Options.grenade_path_esp)
		return;
	if ((type) && path.size()>1)
	{
		Vector nadeStart, nadeEnd;

		Color lineColor(255, 255, 255);
		Vector prev = path[0];
		for (auto it = path.begin(), end = path.end(); it != end; ++it)
		{
			if (Math::WorldToScreen(prev, nadeStart) && Math::WorldToScreen(*it, nadeEnd))
			{
				g_VGuiSurface->DrawSetColor(lineColor);
				g_VGuiSurface->DrawLine((int)nadeStart.x, (int)nadeStart.y, (int)nadeEnd.x, (int)nadeEnd.y);
			}
			prev = *it;
		}

		if (Math::WorldToScreen(prev, nadeEnd))
		{
			//g_DebugOverlay->AddBoxOverlay(nadeEnd, Vector(nadeEnd.x - 2, nadeEnd.y - 2, nadeEnd.z - 2), Vector(nadeEnd.x + 2, nadeEnd.y + 2, nadeEnd.z + 2), QAngle(0, 0, 0), 255, 255, 0, 125, 0.0f);
		}
	}
}

void grenade_prediction::Setup(Vector& vecSrc, Vector& vecThrow, Vector viewangles)
{
	if (!g_Options.grenade_path_esp)
		return;

	Vector angThrow = viewangles;
	auto local = g_LocalPlayer;
	float pitch = angThrow.x;

	if (pitch <= 90.0f)
	{
		if (pitch<-90.0f)
		{
			pitch += 360.0f;
		}
	}
	else
	{
		pitch -= 360.0f;
	}
	float a = pitch - (90.0f - fabs(pitch)) * 10.0f / 90.0f;
	angThrow.x = a;
	float flVel = 750.0f * 0.9f;
	static const float power[] = { 1.0f, 1.0f, 0.5f, 0.0f };
	float b = power[act];
	b = b * 0.7f;
	b = b + 0.3f;
	flVel *= b;

	Vector vForward, vRight, vUp;
	Math::angle_vectors2(angThrow, &vForward, &vRight, &vUp); //angThrow.ToVector(vForward, vRight, vUp);

	vecSrc = g_LocalPlayer->GetEyePos();
	float off = (power[act] * 12.0f) - 12.0f;
	vecSrc.z += off;

	// Game calls UTIL_TraceHull here with hull and assigns vecSrc tr.endpos
	trace_t tr;
	Vector vecDest = vecSrc;
	vecDest += vForward * 22.0f; //vecDest.MultAdd(vForward, 22.0f);

	TraceHull(vecSrc, vecDest, tr);

	// After the hull trace it moves 6 units back along vForward
	// vecSrc = tr.endpos - vForward * 6
	Vector vecBack = vForward; vecBack *= 6.0f;
	vecSrc = tr.endpos;
	vecSrc -= vecBack;

	// Finally calculate velocity
	vecThrow = local->m_vecVelocity(); vecThrow *= 1.25f;
	vecThrow += vForward * flVel; //	vecThrow.MultAdd(vForward, flVel);
}

void grenade_prediction::Simulate(CViewSetup* setup)
{
	if (!g_Options.grenade_path_esp)
		return;
	Vector vecSrc, vecThrow;
	Setup(vecSrc, vecThrow, setup->angles);

	float interval = g_GlobalVars->interval_per_tick;

	// Log positions 20 times per sec
	int logstep = static_cast<int>(0.05f / interval);
	int logtimer = 0;


	path.clear();
	for (unsigned int i = 0; i<path.max_size() - 1; ++i)
	{
		if (!logtimer)
			path.push_back(vecSrc);

		int s = Step(vecSrc, vecThrow, i, interval);
		if ((s & 1)) break;

		// Reset the log timer every logstep OR we bounced
		if ((s & 2) || logtimer >= logstep) logtimer = 0;
		else ++logtimer;
	}
	path.push_back(vecSrc);
}

int grenade_prediction::Step(Vector& vecSrc, Vector& vecThrow, int tick, float interval)
{

	// Apply gravity
	Vector move;
	AddGravityMove(move, vecThrow, interval, false);

	// Push entity
	trace_t tr;
	PushEntity(vecSrc, move, tr);

	int result = 0;
	// Check ending conditions
	if (CheckDetonate(vecThrow, tr, tick, interval))
	{
		result |= 1;
	}

	// Resolve collisions
	if (tr.fraction != 1.0f)
	{
		result |= 2; // Collision!
		ResolveFlyCollisionCustom(tr, vecThrow, interval);
	}

	// Set new position
	vecSrc = tr.endpos;

	return result;
}

bool grenade_prediction::CheckDetonate(const Vector& vecThrow, const trace_t& tr, int tick, float interval)
{
	switch (type)
	{
	case weapon_smokegrenade:
	case weapon_decoy:
		// Velocity must be <0.1, this is only checked every 0.2s
		if (vecThrow.Length2D()<0.1f)
		{
			int det_tick_mod = static_cast<int>(0.2f / interval);
			return !(tick%det_tick_mod);
		}
		return false;

	case weapon_molotov:
	case weapon_incgrenade:
		// Detonate when hitting the floor
		if (tr.fraction != 1.0f && tr.plane.normal.z>0.7f)
			return true;
		// OR we've been flying for too long

	case weapon_flashbang:
	case weapon_hegrenade:
		// Pure timer based, detonate at 1.5s, checked every 0.2s
		return static_cast<float>(tick)*interval>1.5f && !(tick%static_cast<int>(0.2f / interval));

	default:
		assert(false);
		return false;
	}
}

void grenade_prediction::TraceHull(Vector& src, Vector& end, trace_t& tr)
{
	if (!g_Options.grenade_path_esp)
		return;
	Ray_t ray;
	ray.Init(src, end, Vector(-2.0f, -2.0f, -2.0f), Vector(2.0f, 2.0f, 2.0f));

	CTraceFilterWorldAndPropsOnly filter;
	//filter.SetIgnoreClass("BaseCSGrenadeProjectile");
	//filter.bShouldHitPlayers = false;

	g_EngineTrace->TraceRay(ray, 0x200400B, &filter, &tr);
}

void grenade_prediction::AddGravityMove(Vector& move, Vector& vel, float frametime, bool onground)
{
	if (!g_Options.grenade_path_esp)
		return;
	Vector basevel(0.0f, 0.0f, 0.0f);

	move.x = (vel.x + basevel.x) * frametime;
	move.y = (vel.y + basevel.y) * frametime;

	if (onground)
	{
		move.z = (vel.z + basevel.z) * frametime;
	}
	else
	{
		// Game calls GetActualGravity( this );
		float gravity = 800.0f * 0.4f;

		float newZ = vel.z - (gravity * frametime);
		move.z = ((vel.z + newZ) / 2.0f + basevel.z) * frametime;

		vel.z = newZ;
	}
}

void grenade_prediction::PushEntity(Vector& src, const Vector& move, trace_t& tr)
{
	if (!g_Options.grenade_path_esp)
		return;
	Vector vecAbsEnd = src;
	vecAbsEnd += move;

	// Trace through world
	TraceHull(src, vecAbsEnd, tr);
}

void grenade_prediction::ResolveFlyCollisionCustom(trace_t& tr, Vector& vecVelocity, float interval)
{
	if (!g_Options.grenade_path_esp)
		return;
	// Calculate elasticity
	float flSurfaceElasticity = 1.0;  // Assume all surfaces have the same elasticity
	float flGrenadeElasticity = 0.45f; // GetGrenadeElasticity()
	float flTotalElasticity = flGrenadeElasticity * flSurfaceElasticity;
	if (flTotalElasticity>0.9f) flTotalElasticity = 0.9f;
	if (flTotalElasticity<0.0f) flTotalElasticity = 0.0f;

	// Calculate bounce
	Vector vecAbsVelocity;
	PhysicsClipVelocity(vecVelocity, tr.plane.normal, vecAbsVelocity, 2.0f);
	vecAbsVelocity *= flTotalElasticity;

	// Stop completely once we move too slow
	float flSpeedSqr = vecAbsVelocity.LengthSqr();
	static const float flMinSpeedSqr = 20.0f * 20.0f; // 30.0f * 30.0f in CSS
	if (flSpeedSqr<flMinSpeedSqr)
	{
		//vecAbsVelocity.Zero();
		vecAbsVelocity.x = 0.0f;
		vecAbsVelocity.y = 0.0f;
		vecAbsVelocity.z = 0.0f;
	}

	// Stop if on ground
	if (tr.plane.normal.z>0.7f)
	{
		vecVelocity = vecAbsVelocity;
		vecAbsVelocity *= ((1.0f - tr.fraction) * interval); //vecAbsVelocity.Mult((1.0f - tr.fraction) * interval);
		PushEntity(tr.endpos, vecAbsVelocity, tr);
	}
	else
	{
		vecVelocity = vecAbsVelocity;
	}
}

int grenade_prediction::PhysicsClipVelocity(const Vector& in, const Vector& normal, Vector& out, float overbounce)
{
	static const float STOP_EPSILON = 0.1f;

	float    backoff;
	float    change;
	float    angle;
	int        i, blocked;

	blocked = 0;

	angle = normal[2];

	if (angle > 0)
	{
		blocked |= 1;        // floor
	}
	if (!angle)
	{
		blocked |= 2;        // step
	}

	backoff = in.Dot(normal) * overbounce;

	for (i = 0; i<3; i++)
	{
		change = normal[i] * backoff;
		out[i] = in[i] - change;
		if (out[i] > -STOP_EPSILON && out[i] < STOP_EPSILON)
		{
			out[i] = 0;
		}
	}

	return blocked;
}


































































































