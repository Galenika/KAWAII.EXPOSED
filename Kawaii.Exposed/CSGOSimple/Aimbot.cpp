#include "Aimbot.h"
#include "options.hpp"
#include <algorithm>
#include "helpers\math.hpp"
#include "helpers\utils.hpp"
#include "helpers/InputSys.hpp"
#include "hooks.hpp"
#include <string>
#include "autowall.hpp"

#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / g_GlobalVars->interval_per_tick ) )
#define TICKS_TO_TIME( t )		( g_GlobalVars->interval_per_tick *( t ) )

#define LC_NONE				0
#define LC_ALIVE			(1<<0)

#define LC_ORIGIN_CHANGED	(1<<8)
#define LC_ANGLES_CHANGED	(1<<9)
#define LC_SIZE_CHANGED		(1<<10)
#define LC_ANIMATION_CHANGED (1<<11)

#define LAG_COMPENSATION_TELEPORTED_DISTANCE_SQR ( 64.0f * 64.0f )
#define LAG_COMPENSATION_EPS_SQR ( 0.1f * 0.1f )
// Allow 4 units of error ( about 1 / 8 bbox width )
#define LAG_COMPENSATION_ERROR_EPS_SQR ( 4.0f * 4.0f )

#define Assert( _exp ) ((void)0)
int aimspot;
bool standalone_rcs = false;

float GetFOV(const QAngle& viewAngle, const QAngle& aimAngle)
{
	QAngle delta = aimAngle - viewAngle;
	Math::NormalizeAngles(delta);
	return sqrtf(powf(delta.pitch, 2.0f) + powf(delta.yaw, 2.0f));
}

namespace Aimbot
{
	//float GetHitchance()
	//{
	//	float hitchance = 101;
	//	if (!g_LocalPlayer->m_hActiveWeapon())
	//		return 0.0f;

	//	if (g_Options.hitchance > 1)
	//	{
	//		float inaccuracy = g_LocalPlayer->m_hActiveWeapon()->GetInaccuracy();
	//		if (inaccuracy == 0) inaccuracy = 0.0000001;
	//		inaccuracy = 1 / inaccuracy;
	//		hitchance = inaccuracy;

	//	}

	//	return hitchance;
	//}

	bool doNoRCS(CUserCmd* pCmd)
	{
		if (g_EngineClient->IsInGame() && g_LocalPlayer->IsAlive() && g_Options.no_recoil == true)
		{
			auto punchAngles = g_LocalPlayer->m_aimPunchAngle() * 2;
			if (punchAngles.pitch != 0.0f || punchAngles.yaw != 0)
			{
				pCmd->viewangles -= punchAngles;
				if (g_Options.antiuntrusted)
				{
					Math::NormalizeAngles(pCmd->viewangles);
					Math::ClampAngles(pCmd->viewangles);
				}
				return false;
			}
		}
	}

	//bool testRCS(CUserCmd* pCmd)
	//{
	//	QAngle ViewAngle;
	//	if (g_EngineClient->IsInGame() && g_LocalPlayer->IsAlive())
	//	{
	//		pCmd->viewangles -= g_LocalPlayer->m_aimPunchAngle() * 2.f;
	//		Math::NormalizeAngles(pCmd->viewangles);
	//	}
	//	return false;
	//}

	QAngle AimbotCalcAngle(Vector src, Vector dst)
	{
		QAngle ret;
		Vector delta = src - dst;
		double hyp = delta.Length2D(); //delta.Length
		ret.yaw = (atan(delta.y / delta.x) * 57.295779513082f);
		ret.pitch = (atan(delta.z / hyp) * 57.295779513082f);
		ret[2] = 0.00;
		if (delta.x >= 0.0) ret.yaw += 180.0f;
		return ret;
	}

	QAngle Smooth(QAngle finalangle, QAngle viewangles)
	{
		float smooth, targetsmooth = 0.0f;

		auto pWeapon = g_LocalPlayer->m_hActiveWeapon();

		if (!pWeapon) return viewangles;

		if (pWeapon->IsPistol()) targetsmooth = g_Options.pistol_aimbot_smoothness;
		else if (pWeapon->IsRifle()) targetsmooth = g_Options.rifle_aimbot_smoothness;
		else if (pWeapon->IsSniper()) targetsmooth = g_Options.sniper_aimbot_smoothness;
		else if (pWeapon->IsShotgun()) targetsmooth = g_Options.shotgun_aimbot_smoothness;
		else if (pWeapon->IsSMG()) targetsmooth = g_Options.smg_aimbot_smoothness;

		QAngle delta = finalangle - viewangles;
		Math::NormalizeAngles(delta);
		Math::ClampAngles(delta);
		if (targetsmooth > 0.0f) smooth = powf(targetsmooth, 0.4f);
		else if (targetsmooth <= 0.0f) smooth = powf(0.0001f, 0.4f);
		smooth = std::min(0.99f, smooth);
		QAngle toChange = delta - delta * smooth;
		return toChange;
	}


	void doRCS(CUserCmd* pCmd)
	{
		static QAngle RCSLastPunch;
		if (g_EngineClient->IsInGame() && g_LocalPlayer->IsAlive())
		{
			auto pWeapon = g_LocalPlayer->m_hActiveWeapon();

			if (!pWeapon) return;

			if (!(pCmd->buttons & IN_ATTACK))
				return;

			// standalone rcs 
			if (pWeapon->IsPistol() && g_Options.pistol_standalone_rcs != true) return;
			if (pWeapon->IsRifle() && g_Options.rifle_standalone_rcs != true) return;
			if (pWeapon->IsSniper() && g_Options.sniper_standalone_rcs != true) return;
			if (pWeapon->IsShotgun() && g_Options.shotgun_standalone_rcs != true) return;
			if (pWeapon->IsSMG() && g_Options.smg_standalone_rcs != true) return;
			standalone_rcs = true;

			// rcs disabled check
			if (pWeapon->IsPistol() && g_Options.pistol_rcs_amount <= 0.0f) return;
			else if (pWeapon->IsRifle() && g_Options.rifle_rcs_amount <= 0.0f) return;
			else if (pWeapon->IsSniper() && g_Options.sniper_rcs_amount <= 0.0f) return;
			else if (pWeapon->IsShotgun() && g_Options.shotgun_rcs_amount <= 0.0f) return;
			else if (pWeapon->IsSMG() && g_Options.smg_rcs_amount <= 0.0f) return;

			// rcs setting
			static float rcs = 0.0f;
			if (pWeapon->IsPistol()) rcs = g_Options.pistol_rcs_amount;
			else if (pWeapon->IsRifle()) rcs = g_Options.rifle_rcs_amount;
			else if (pWeapon->IsSniper()) rcs = g_Options.sniper_rcs_amount;
			else if (pWeapon->IsShotgun()) rcs = g_Options.shotgun_rcs_amount;
			else if (pWeapon->IsSMG()) rcs = g_Options.smg_rcs_amount;

			static bool silent = false;
			if (pWeapon->IsPistol()) silent = g_Options.pistol_aimbot_silent;
			else if (pWeapon->IsRifle()) silent = g_Options.rifle_aimbot_silent;
			else if (pWeapon->IsSniper()) silent = g_Options.sniper_aimbot_silent;
			else if (pWeapon->IsShotgun()) silent = g_Options.shotgun_aimbot_silent;
			else if (pWeapon->IsSMG()) silent = g_Options.smg_aimbot_silent;

			static QAngle OldPunch;

			QAngle ViewAngle;
			g_EngineClient->GetViewAngles(ViewAngle);

			if (g_LocalPlayer->m_iShotsFired() > 1)
			{
				QAngle NewPunch = QAngle(g_LocalPlayer->m_aimPunchAngle().pitch - OldPunch.pitch, g_LocalPlayer->m_aimPunchAngle().yaw - OldPunch.yaw, 0);
				ViewAngle -= NewPunch * rcs;
				Math::NormalizeAngles(ViewAngle);
				Math::ClampAngles(ViewAngle);
				if (!silent)
				{
					pCmd->viewangles = ViewAngle;
					g_EngineClient->SetViewAngles(ViewAngle);
				}
				else pCmd->viewangles = ViewAngle;
			}

			Math::NormalizeAngles(pCmd->viewangles);
			Math::ClampAngles(pCmd->viewangles);

			OldPunch = g_LocalPlayer->m_aimPunchAngle();
		}
	}

	C_BasePlayer* GetClosestPlayerToCrosshair()
	{
		float bestFov = 1000000000.0f;
		C_BasePlayer* closestEntity = nullptr;
		auto pWeapon = g_LocalPlayer->m_hActiveWeapon();

		if (!pWeapon) return nullptr;

		// aimbot disabled check
		if (pWeapon->IsPistol() && g_Options.pistol_aimbot_AimbotFOV <= 0.0f) return nullptr;
		else if (pWeapon->IsRifle() && g_Options.rifle_aimbot_AimbotFOV <= 0.0f) return nullptr;
		else if (pWeapon->IsSniper() && g_Options.sniper_aimbot_AimbotFOV <= 0.0f) return nullptr;
		else if (pWeapon->IsShotgun() && g_Options.shotgun_aimbot_AimbotFOV <= 0.0f) return nullptr;
		else if (pWeapon->IsSMG() && g_Options.smg_aimbot_AimbotFOV <= 0.0f) return nullptr;

		// setiting max fov
		if (pWeapon->IsPistol() && g_Options.pistol_aimbot_AimbotFOV > 0) bestFov = g_Options.pistol_aimbot_AimbotFOV;
		else if (pWeapon->IsRifle() && g_Options.rifle_aimbot_AimbotFOV > 0) bestFov = g_Options.rifle_aimbot_AimbotFOV;
		else if (pWeapon->IsSniper() && g_Options.sniper_aimbot_AimbotFOV > 0) bestFov = g_Options.sniper_aimbot_AimbotFOV;
		else if (pWeapon->IsShotgun() && g_Options.shotgun_aimbot_AimbotFOV > 0) bestFov = g_Options.shotgun_aimbot_AimbotFOV;
		else if (pWeapon->IsSMG() && g_Options.smg_aimbot_AimbotFOV > 0) bestFov = g_Options.smg_aimbot_AimbotFOV;

		if (!g_LocalPlayer) return nullptr;

		for (int i = 1; i < g_EngineClient->GetMaxClients(); ++i)
		{
			C_BasePlayer* player = (C_BasePlayer*)g_EntityList->GetClientEntity(i);

			if (player
				&& player != g_LocalPlayer
				&& !player->IsDormant()
				&& player->IsAlive()
				&& !player->m_bGunGameImmunity())
			{
				if (player->m_iTeamNum() != g_LocalPlayer->m_iTeamNum())
				{
					player_info_t entityInformation;
					g_EngineClient->GetPlayerInfo(i, &entityInformation);

					Vector eVecTarget = player->GetBonePos(aimspot);
					Vector pVecTarget = g_LocalPlayer->GetEyePos();

					QAngle viewAngles;
					g_EngineClient->GetViewAngles(viewAngles);

					float distance = pVecTarget.DistTo(eVecTarget);
					float fov = GetFOV(viewAngles, Math::CalcAngle1(pVecTarget, eVecTarget));
					int hp = player->m_iHealth();

					if (fov < bestFov)
					{
						closestEntity = player;
						bestFov = fov;
					}
				}
			}
		}
		return closestEntity;
	}

	C_AutoWall* Autowall = new C_AutoWall();

	void doAimbot(CUserCmd* pCmd)
	{
		auto pClosest = GetClosestPlayerToCrosshair();
		auto pWeapon = g_LocalPlayer->m_hActiveWeapon();

		if (!pClosest) return;

		if (pWeapon) {

			if (pWeapon->IsKnife() || pWeapon->IsGrenade() || pWeapon->m_iItemDefinitionIndex() == weapon_c4) return;

			// aimbot disabled check
			if (pWeapon->IsPistol() && g_Options.pistol_aimbot_AimbotFOV <= 0.0f) return;
			else if (pWeapon->IsRifle() && g_Options.rifle_aimbot_AimbotFOV <= 0.0f) return;
			else if (pWeapon->IsSniper() && g_Options.sniper_aimbot_AimbotFOV <= 0.0f) return;
			else if (pWeapon->IsShotgun() && g_Options.shotgun_aimbot_AimbotFOV <= 0.0f) return;
			else if (pWeapon->IsSMG() && g_Options.smg_aimbot_AimbotFOV <= 0.0f) return;

			// aimbot key not press check
			if (pWeapon->IsPistol() && g_Options.pistol_aimbot_key > 0 && !InputSys::Get().IsKeyDown(g_Options.pistol_aimbot_key)) return;
			else if (pWeapon->IsRifle() && g_Options.rifle_aimbot_key > 0 && !InputSys::Get().IsKeyDown(g_Options.rifle_aimbot_key)) return;
			else if (pWeapon->IsSniper() && g_Options.sniper_aimbot_key > 0 && !InputSys::Get().IsKeyDown(g_Options.sniper_aimbot_key)) return;
			else if (pWeapon->IsShotgun() && g_Options.shotgun_aimbot_key > 0 && !InputSys::Get().IsKeyDown(g_Options.shotgun_aimbot_key)) return;
			else if (pWeapon->IsSMG() && g_Options.smg_aimbot_key > 0 && !InputSys::Get().IsKeyDown(g_Options.smg_aimbot_key)) return;

			// vischeck
			if (pWeapon->IsPistol() && g_Options.pistol_vischeck && !g_LocalPlayer->CanSeePlayer(pClosest, HITBOX_CHEST)) return;
			else if (pWeapon->IsRifle() && g_Options.rifle_vischeck && !g_LocalPlayer->CanSeePlayer(pClosest, HITBOX_CHEST)) return;
			else if (pWeapon->IsSniper() && g_Options.sniper_vischeck && !g_LocalPlayer->CanSeePlayer(pClosest, HITBOX_CHEST)) return;
			else if (pWeapon->IsShotgun() && g_Options.shotgun_vischeck && !g_LocalPlayer->CanSeePlayer(pClosest, HITBOX_CHEST)) return;
			else if (pWeapon->IsSMG() && g_Options.smg_vischeck && !g_LocalPlayer->CanSeePlayer(pClosest, HITBOX_CHEST)) return;

			if (pWeapon->IsPistol() && g_Options.pistol_vischeck && !g_LocalPlayer->CanSeePlayer(pClosest, HITBOX_HEAD)) return;
			else if (pWeapon->IsRifle() && g_Options.rifle_vischeck && !g_LocalPlayer->CanSeePlayer(pClosest, HITBOX_HEAD)) return;
			else if (pWeapon->IsSniper() && g_Options.sniper_vischeck && !g_LocalPlayer->CanSeePlayer(pClosest, HITBOX_HEAD)) return;
			else if (pWeapon->IsShotgun() && g_Options.shotgun_vischeck && !g_LocalPlayer->CanSeePlayer(pClosest, HITBOX_HEAD)) return;
			else if (pWeapon->IsSMG() && g_Options.smg_vischeck && !g_LocalPlayer->CanSeePlayer(pClosest, HITBOX_HEAD)) return;

			Vector BonePos = pClosest->GetBonePos(aimspot);
			QAngle getAimbotTargetAngle = AimbotCalcAngle(g_LocalPlayer->GetEyePos(), BonePos);
			//if (pClosest && pClosest->m_iTeamNum() != g_LocalPlayer->m_iTeamNum() && pClosest->IsAlive())
			/*{


				float Damage = 0;
				Autowall->PenetrateWall(g_LocalPlayer, pWeapon, BonePos, Damage);

				if (g_Options.hitchance <= GetHitchance())
				{
					if (g_Options.autowall)
					{
						if (Damage >= g_Options.autowall_min_damage)
						{
							static bool silent = false;

							if (pWeapon->IsPistol()) silent = g_Options.pistol_aimbot_silent;
							else if (pWeapon->IsRifle()) silent = g_Options.rifle_aimbot_silent;
							else if (pWeapon->IsSniper()) silent = g_Options.sniper_aimbot_silent;
							else if (pWeapon->IsShotgun()) silent = g_Options.shotgun_aimbot_silent;
							else if (pWeapon->IsSMG()) silent = g_Options.smg_aimbot_silent;

							if (!silent)
							{
								QAngle ViewAngles;
								g_EngineClient->GetViewAngles(ViewAngles);
								QAngle ToChange = Smooth(getAimbotTargetAngle, ViewAngles);
								QAngle Final = ViewAngles + ToChange;
								Math::NormalizeAngles(Final);
								Math::ClampAngles(Final);
								g_EngineClient->SetViewAngles(Final);
							}
							else
							{
								bool canShoot = pWeapon->m_flNextPrimaryAttack() <= g_GlobalVars->curtime && !pWeapon->IsReloading();
								if (canShoot) pCmd->viewangles = getAimbotTargetAngle;
							}

							Math::NormalizeAngles(pCmd->viewangles);
							Math::ClampAngles(pCmd->viewangles);

							if (g_Options.autoshoot)
							{
								if (!(pCmd->buttons & IN_USE))
								{
									if (pWeapon->m_flNextPrimaryAttack() > g_GlobalVars->curtime)
									{
										if (pWeapon->m_iItemDefinitionIndex() == weapon_revolver)
											pCmd->buttons &= ~IN_ATTACK2;
										else
											pCmd->buttons &= ~IN_ATTACK;
									}
								}
							}

							if (!(pCmd->buttons & IN_USE))
							{
								if (!(pWeapon->m_flNextPrimaryAttack() > g_GlobalVars->curtime))
								{
									if (g_Options.autoscope && pWeapon->GetCSWeaponData()->iZoomLevels > 0 && !g_LocalPlayer->m_bIsScoped())
										pCmd->buttons |= IN_ATTACK2;
									else if (pWeapon->m_iItemDefinitionIndex() == weapon_revolver)
										pCmd->buttons |= IN_ATTACK2;
									else if (g_Options.autoshoot)
										pCmd->buttons |= IN_ATTACK;
								}
							}
						}
					}*/
			if (pClosest && pClosest->m_iTeamNum() != g_LocalPlayer->m_iTeamNum() && pClosest->IsAlive())
			{
				static bool silent = false;

				if (pWeapon->IsPistol()) silent = g_Options.pistol_aimbot_silent;
				else if (pWeapon->IsRifle()) silent = g_Options.rifle_aimbot_silent;
				else if (pWeapon->IsSniper()) silent = g_Options.sniper_aimbot_silent;
				else if (pWeapon->IsShotgun()) silent = g_Options.shotgun_aimbot_silent;
				else if (pWeapon->IsSMG()) silent = g_Options.smg_aimbot_silent;

				// rcs setting
				static float rcs = 0.0f;
				if (pWeapon->IsPistol()) rcs = g_Options.pistol_rcs_amount;
				else if (pWeapon->IsRifle()) rcs = g_Options.rifle_rcs_amount;
				else if (pWeapon->IsSniper()) rcs = g_Options.sniper_rcs_amount;
				else if (pWeapon->IsShotgun()) rcs = g_Options.shotgun_rcs_amount;
				else if (pWeapon->IsSMG()) rcs = g_Options.smg_rcs_amount;

				float targetsmooth = 0.0f;
				if (pWeapon->IsPistol()) targetsmooth = g_Options.pistol_aimbot_smoothness;
				else if (pWeapon->IsRifle()) targetsmooth = g_Options.rifle_aimbot_smoothness;
				else if (pWeapon->IsSniper()) targetsmooth = g_Options.sniper_aimbot_smoothness;
				else if (pWeapon->IsShotgun()) targetsmooth = g_Options.shotgun_aimbot_smoothness;
				else if (pWeapon->IsSMG()) targetsmooth = g_Options.smg_aimbot_smoothness;

				if (!silent && standalone_rcs)
				{
					QAngle ViewAngles;
					g_EngineClient->GetViewAngles(ViewAngles);
					QAngle AimbotTargetAngle1 = getAimbotTargetAngle;
					QAngle AimbotRCS = AimbotTargetAngle1 -= g_LocalPlayer->m_aimPunchAngle() * rcs;
					if (targetsmooth > 0.0f)
					{
						QAngle ToChange = Smooth(AimbotRCS, ViewAngles);
						QAngle Final = ViewAngles + ToChange;
						Math::NormalizeAngles(Final);
						Math::ClampAngles(Final);
						g_EngineClient->SetViewAngles(Final);
					}
					else
					{
						QAngle Final;

						if (g_LocalPlayer->m_iShotsFired() > 0)
							Final = AimbotRCS;
						else
							Final = getAimbotTargetAngle;

						Math::NormalizeAngles(Final);
						Math::ClampAngles(Final);
						g_EngineClient->SetViewAngles(Final);
					}
				}
				else if (!silent)
				{
					//QAngle ViewAngles;
					//g_EngineClient->GetViewAngles(ViewAngles);
					//QAngle AimbotTargetAngle1 = getAimbotTargetAngle;
					//QAngle ToChange = Smooth(AimbotTargetAngle1, ViewAngles);
					//QAngle Final = ViewAngles + ToChange;
					//g_EngineClient->SetViewAngles(Final);
					//Math::NormalizeAngles(Final);
					//Math::ClampAngles(Final);
					QAngle ViewAngles;
					g_EngineClient->GetViewAngles(ViewAngles);
					QAngle AimbotTargetAngle1 = getAimbotTargetAngle;
					QAngle AimbotRCS = AimbotTargetAngle1 -= g_LocalPlayer->m_aimPunchAngle() * rcs;
					if (targetsmooth > 0.0f)
					{
						QAngle ToChange = Smooth(AimbotRCS, ViewAngles);
						QAngle Final = ViewAngles + ToChange;
						Math::NormalizeAngles(Final);
						Math::ClampAngles(Final);
						g_EngineClient->SetViewAngles(Final);
					}
					else
					{
						QAngle Final;

						if (g_LocalPlayer->m_iShotsFired() > 0)
							Final = AimbotRCS;
						else
							Final = getAimbotTargetAngle;

						Math::NormalizeAngles(Final);
						Math::ClampAngles(Final);
						g_EngineClient->SetViewAngles(Final);
					}				
				}
				else if (silent)
				{
					if (pCmd->buttons & IN_ATTACK)
					{
						bool canShoot = pWeapon->m_flNextPrimaryAttack() <= g_GlobalVars->curtime && !pWeapon->IsReloading();

						QAngle ViewAngles;
						g_EngineClient->GetViewAngles(ViewAngles);
						QAngle AimbotTargetAngle1 = getAimbotTargetAngle;
						QAngle AimbotRCS = AimbotTargetAngle1 -= g_LocalPlayer->m_aimPunchAngle() * rcs;

						static bool JustShot = false;

						if (canShoot && !JustShot)
						{
							QAngle Final;

							if (g_LocalPlayer->m_iShotsFired() > 0)
								Final = AimbotRCS;
							else
								Final = getAimbotTargetAngle;

							Math::NormalizeAngles(Final);
							Math::ClampAngles(Final);

							sendPacket = false;

							pCmd->viewangles = Final;

							JustShot = true;
						}
						else if (JustShot)
						{
							sendPacket = true;
							JustShot = false;
						}
					}
				}

				Math::NormalizeAngles(pCmd->viewangles);
				Math::ClampAngles(pCmd->viewangles);

				/*
				if (g_Options.autoshoot)
				{
					if (!(pCmd->buttons & IN_USE))
					{
						if (pWeapon->m_flNextPrimaryAttack() > g_GlobalVars->curtime)
						{
							if (pWeapon->m_iItemDefinitionIndex() == weapon_revolver)
								pCmd->buttons &= ~IN_ATTACK2;
							else
								pCmd->buttons &= ~IN_ATTACK;
						}
					}
				}
				*/

				/*
				if (!(pCmd->buttons & IN_USE))
				{
					if (!(pWeapon->m_flNextPrimaryAttack() > g_GlobalVars->curtime))
					{
						if (g_Options.autoscope && pWeapon->GetCSWeaponData()->iZoomLevels > 0 && !g_LocalPlayer->m_bIsScoped())
							pCmd->buttons |= IN_ATTACK2;
						else if (pWeapon->m_iItemDefinitionIndex() == weapon_revolver)
							pCmd->buttons |= IN_ATTACK2;
						else if (g_Options.autoshoot)
							pCmd->buttons |= IN_ATTACK;
					}
				}
				*/
			}
		}
	}
}





































































































