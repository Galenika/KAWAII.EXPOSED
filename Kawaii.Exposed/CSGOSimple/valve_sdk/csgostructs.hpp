#pragma once

#include "sdk.hpp"

#define NETVAR(type, name, table, netvar)                           \
    type& name##() const {                                          \
        static int _##name = NetvarSys::Get().GetOffset(table, netvar);     \
        return *(type*)((uintptr_t)this + _##name);                 \
    }

#define PNETVAR(type, name, table, netvar)                           \
    type* name##() const {                                          \
        static int _##name = NetvarSys::Get().GetOffset(table, netvar);     \
        return (type*)((uintptr_t)this + _##name);                 \
    }

enum CSWeaponType
{
    WEAPONTYPE_KNIFE = 0,
    WEAPONTYPE_PISTOL,
    WEAPONTYPE_SUBMACHINEGUN,
    WEAPONTYPE_RIFLE,
    WEAPONTYPE_SHOTGUN,
    WEAPONTYPE_SNIPER_RIFLE,
    WEAPONTYPE_MACHINEGUN,
    WEAPONTYPE_C4,
	WEAPONTYPE_PLACEHOLDER,
    WEAPONTYPE_GRENADE,
    WEAPONTYPE_UNKNOWN
};

class C_BaseEntity;

// Created with ReClass.NET by KN4CK3R
class CHudTexture
{
public:
    char szShortName[64];    //0x0000
    char szTextureFile[64];  //0x0040
    bool bRenderUsingFont;   //0x0080
    bool bPrecached;         //0x0081
    int8_t cCharacterInFont; //0x0082
    uint8_t pad_0083[1];     //0x0083
    uint32_t hFont;          //0x0084
    int32_t iTextureId;      //0x0088
    float afTexCoords[4];    //0x008C
    uint8_t pad_009C[16];    //0x009C
};

#pragma pack(push, 1)
class CCSWeaponInfo
{
public:
	char pad_0x0000[4]; // 0x0000
	char* consoleName; // 0x0004 
	char pad_0008[12]; // 0x0008 
	int iMaxClip1; // 0x0014 
	int iMaxClip2; // 0x0018 
	int iDefaultClip1; // 0x001C 
	int iDefaultClip2; // 0x0020 
	char pad_0024[8]; // 0x0024 
	char* szWorldModel; // 0x002C 
	char* szViewModel; // 0x0030 
	char* szDroppedModel; // 0x0034 
	char pad_0038[4]; // 0x0038 
	char* N0000023E; // 0x003C 
	char pad_0040[56]; // 0x0040 
	char* szEmptySound; // 0x0078 
	char pad_007C[4]; // 0x007C 
	char* szBulletType; // 0x0080 
	char pad_0084[4]; // 0x0084 
	char* szHudName; // 0x0088 
	char* szWeaponName; // 0x008C 
	char pad_0090[56]; // 0x0090 
	int WeaponType; // 0x00C8 
	int iWeaponPrice; // 0x00CC 
	int iKillAward; // 0x00D0 
	char* szAnimationPrefix; // 0x00D4 
	float flCycleTime; // 0x00D8 
	float flCycleTimeAlt; // 0x00DC 
	float flTimeToIdle; // 0x00E0 
	float flIdleInterval; // 0x00E4 
	bool bFullAuto; // 0x00E8 
	char pad_0x00E5[3]; // 0x00E9 
	int iDamage; // 0x00EC 
	float flArmorRatio; // 0x00F0 
	int iBullets; // 0x00F4 
	float flPenetration; // 0x00F8 
	float flFlinchVelocityModifierLarge; // 0x00FC 
	float flFlinchVelocityModifierSmall; // 0x0100 
	float flRange; // 0x0104 
	float flRangeModifier; // 0x0108 
	float flThrowVelocity; // 0x010C 
	char pad_0x010C[12]; // 0x0110 
	bool bHasSilencer; // 0x011C 
	char pad_0x0119[3]; // 0x011D 
	char* pSilencerModel; // 0x0120 
	int iCrosshairMinDistance; // 0x0124 
	int iCrosshairDeltaDistance;// 0x0128 - iTeam? 
	float flMaxPlayerSpeed; // 0x012C 
	float flMaxPlayerSpeedAlt; // 0x0130 
	float flSpread; // 0x0134 
	float flSpreadAlt; // 0x0138 
	float flInaccuracyCrouch; // 0x013C 
	float flInaccuracyCrouchAlt; // 0x0140 
	float flInaccuracyStand; // 0x0144 
	float flInaccuracyStandAlt; // 0x0148 
	float flInaccuracyJumpInitial;// 0x014C 
	float flInaccuracyJump; // 0x0150 
	float flInaccuracyJumpAlt; // 0x0154 
	float flInaccuracyLand; // 0x0158 
	float flInaccuracyLandAlt; // 0x015C 
	float flInaccuracyLadder; // 0x0160 
	float flInaccuracyLadderAlt; // 0x0164 
	float flInaccuracyFire; // 0x0168 
	float flInaccuracyFireAlt; // 0x016C 
	float flInaccuracyMove; // 0x0170 
	float flInaccuracyMoveAlt; // 0x0174 
	float flInaccuracyReload; // 0x0178 
	int iRecoilSeed; // 0x017C 
	float flRecoilAngle; // 0x0180 
	float flRecoilAngleAlt; // 0x0184 
	float flRecoilAngleVariance; // 0x0188 
	float flRecoilAngleVarianceAlt; // 0x018C 
	float flRecoilMagnitude; // 0x0190 
	float flRecoilMagnitudeAlt; // 0x0194 
	float flRecoilMagnitudeVariance; // 0x0198 
	float flRecoilMagnitudeVarianceAlt; // 0x019C 
	float flRecoveryTimeCrouch; // 0x01A0 
	float flRecoveryTimeStand; // 0x01A4 
	float flRecoveryTimeCrouchFinal; // 0x01A8 
	float flRecoveryTimeStandFinal; // 0x01AC 
	int iRecoveryTransitionStartBullet;// 0x01B0 
	int iRecoveryTransitionEndBullet; // 0x01B4 
	bool bUnzoomAfterShot; // 0x01B8 
	bool bHideViewModelZoomed; // 0x01B9 
	char pad_0x01B5[2]; // 0x01BA 
	char iZoomLevels[3]; // 0x01BC 
	int iZoomFOV[2]; // 0x01C0 
	float fZoomTime[3]; // 0x01C4 
	char* szWeaponClass; // 0x01D4 
	float flAddonScale; // 0x01D8 
	char pad_0x01DC[4]; // 0x01DC 
	char* szEjectBrassEffect; // 0x01E0 
	char* szTracerEffect; // 0x01E4 
	int iTracerFrequency; // 0x01E8 
	int iTracerFrequencyAlt; // 0x01EC 
	char* szMuzzleFlashEffect_1stPerson; // 0x01F0 
	char pad_0x01F4[4]; // 0x01F4 
	char* szMuzzleFlashEffect_3rdPerson; // 0x01F8 
	char pad_0x01FC[4]; // 0x01FC 
	char* szMuzzleSmokeEffect; // 0x0200 
	float flHeatPerShot; // 0x0204 
	char* szZoomInSound; // 0x0208 
	char* szZoomOutSound; // 0x020C 
	float flInaccuracyPitchShift; // 0x0210 
	float flInaccuracySoundThreshold; // 0x0214 
	float flBotAudibleRange; // 0x0218 
	BYTE pad_0x0218[8]; // 0x0220 
	char* pWrongTeamMsg; // 0x0224 
	bool bHasBurstMode; // 0x0228 
	BYTE pad_0x0225[3]; // 0x0229 
	bool bIsRevolver; // 0x022C 
	bool bCannotShootUnderwater; // 0x0230
}; //Size: 0x0828
#pragma pack(pop)

class C_EconItemView
{
public:
    NETVAR(int32_t, m_bInitialized,          "DT_BaseAttributableItem", "m_bInitialized");
    NETVAR(int32_t, m_iItemDefinitionIndex,  "DT_BaseAttributableItem", "m_iItemDefinitionIndex");
    NETVAR(int32_t, m_iEntityLevel,          "DT_BaseAttributableItem", "m_iEntityLevel");
    NETVAR(int32_t, m_iAccountID,            "DT_BaseAttributableItem", "m_iAccountID");
    NETVAR(int32_t, m_iItemIDLow,            "DT_BaseAttributableItem", "m_iItemIDLow");
    NETVAR(int32_t, m_iItemIDHigh,           "DT_BaseAttributableItem", "m_iItemIDHigh");
    NETVAR(int32_t, m_iEntityQuality,        "DT_BaseAttributableItem", "m_iEntityQuality");
};

class C_BaseEntity : public IClientEntity
{
public:

    static __forceinline C_BaseEntity* GetEntityByIndex(int index)
    {
        return static_cast<C_BaseEntity*>(g_EntityList->GetClientEntity(index));
    }
    static __forceinline C_BaseEntity* get_entity_from_handle(CBaseHandle h)
    {
        return static_cast<C_BaseEntity*>(g_EntityList->GetClientEntityFromHandle(h));
    }


    NETVAR(int32_t,               m_nModelIndex,    "DT_BaseEntity",  "m_nModelIndex");
    NETVAR(int32_t,               m_iTeamNum,       "DT_BaseEntity",  "m_iTeamNum");
    NETVAR(Vector,                m_vecOrigin,      "DT_BaseEntity",  "m_vecOrigin");
    NETVAR(bool,                  m_bShouldGlow,    "DT_DynamicProp", "m_bShouldGlow");
    NETVAR(CHandle<C_BasePlayer>, m_hOwnerEntity,   "DT_BaseEntity",  "m_hOwnerEntity");
    
    const matrix3x4_t& m_rgflCoordinateFrame()
    {
        static auto _m_rgflCoordinateFrame = NetvarSys::Get().GetOffset("DT_BaseEntity", "m_CollisionGroup") - 0x30;
        return *(matrix3x4_t*)((uintptr_t)this + _m_rgflCoordinateFrame);
    }

    bool IsPlayer();
    bool IsWeapon();
    bool IsPlantedC4();
    bool IsDefuseKit();
};

class C_PlantedC4
{
public:
    NETVAR(bool,                  m_bBombTicking,      "DT_PlantedC4", "m_bBombTicking");
    NETVAR(bool,                  m_bBombDefused,      "DT_PlantedC4", "m_bBombDefused");
    NETVAR(float,                 m_flC4Blow,          "DT_PlantedC4", "m_flC4Blow");
    NETVAR(float,                 m_flTimerLength,     "DT_PlantedC4", "m_flTimerLength");
    NETVAR(float,                 m_flDefuseLength,    "DT_PlantedC4", "m_flDefuseLength");
    NETVAR(float,                 m_flDefuseCountDown, "DT_PlantedC4", "m_flDefuseCountDown");
    NETVAR(CHandle<C_BasePlayer>, m_hBombDefuser,      "DT_PlantedC4", "m_hBombDefuser");
};

class C_BaseAttributableItem : public C_BaseEntity
{
public:
	NETVAR(int32_t, m_iItemDefinitionIndex, "DT_BaseAttributableItem", "m_iItemDefinitionIndex");

    NETVAR(uint64_t, m_OriginalOwnerXuid,    "DT_BaseAttributableItem", "m_OriginalOwnerXuidLow");
    NETVAR(int32_t, m_OriginalOwnerXuidLow,  "DT_BaseAttributableItem", "m_OriginalOwnerXuidLow");
    NETVAR(int32_t, m_OriginalOwnerXuidHigh, "DT_BaseAttributableItem", "m_OriginalOwnerXuidHigh");
    NETVAR(int32_t, m_nFallbackStatTrak,     "DT_BaseAttributableItem", "m_nFallbackStatTrak");
    NETVAR(int32_t, m_nFallbackPaintKit,     "DT_BaseAttributableItem", "m_nFallbackPaintKit");
    NETVAR(int32_t, m_nFallbackSeed,         "DT_BaseAttributableItem", "m_nFallbackSeed");
    NETVAR(float_t, m_flFallbackWear,        "DT_BaseAttributableItem", "m_flFallbackWear");
	NETVAR(int32_t, m_iItemIDLow, "DT_BaseAttributableItem", "m_iItemIDLow");
	NETVAR(int32_t, m_iItemIDHigh, "DT_BaseAttributableItem", "m_iItemIDHigh");
	NETVAR(int32_t, m_iEntityQuality, "DT_BaseAttributableItem", "m_iEntityQuality");
	NETVAR(char32_t, m_szCustomName, "DT_BaseAttributableItem", "m_szCustomName");


    C_EconItemView& m_Item()
    {
        // Cheating. It should be this + m_Item netvar but then the netvars inside C_EconItemView wont work properly.
        // A real fix for this requires a rewrite of the netvar manager
        return *(C_EconItemView*)this;
    }
};


class C_BaseCombatWeapon : public C_BaseAttributableItem
{
public:
    NETVAR(float_t, m_flNextPrimaryAttack,   "DT_BaseCombatWeapon", "m_flNextPrimaryAttack");
    NETVAR(float_t, m_flNextSecondaryAttack, "DT_BaseCombatWeapon", "m_flNextSecondaryAttack");
    NETVAR(int32_t, m_iClip1,                "DT_BaseCombatWeapon", "m_iClip1");
    NETVAR(int32_t, m_iClip2,                "DT_BaseCombatWeapon", "m_iClip2");
    NETVAR(float_t, m_flRecoilIndex,         "DT_WeaponCSBase", "m_flRecoilIndex");

    CCSWeaponInfo* GetCSWeaponData();
    bool HasBullets();
    bool CanFire();
    bool IsGrenade();
    bool IsKnife();
    bool IsReloading();
	bool IsRifle();
	bool IsPistol();
	bool IsSniper();
	bool IsShotgun();
	bool IsSMG();
    float GetInaccuracy();
    float GetSpread();
    void UpdateAccuracyPenalty();
};

class C_BasePlayer : public C_BaseEntity
{
public:
    static __forceinline C_BasePlayer* GetPlayerByUserId(int id)
    {
        return static_cast<C_BasePlayer*>(GetEntityByIndex(g_EngineClient->GetPlayerForUserID(id)));
    }
    static __forceinline C_BasePlayer* GetPlayerByIndex(int i)
    {
        return static_cast<C_BasePlayer*>(GetEntityByIndex(i));
    }

    NETVAR(bool,                             m_bHasDefuser,          "DT_CSPlayer", "m_bHasDefuser");
    NETVAR(bool,                             m_bGunGameImmunity,     "DT_CSPlayer", "m_bGunGameImmunity");
    NETVAR(int32_t,                          m_iShotsFired,          "DT_CSPlayer", "m_iShotsFired");
    NETVAR(QAngle,                           m_angEyeAngles,         "DT_CSPlayer", "m_angEyeAngles[0]");
    NETVAR(int,                              m_ArmorValue,           "DT_CSPlayer", "m_ArmorValue");
    NETVAR(bool,                             m_bHasHelmet,           "DT_CSPlayer", "m_bHasHelmet");
    NETVAR(bool,                             m_bIsScoped,            "DT_CSPlayer", "m_bIsScoped");;
    NETVAR(float,                            m_flLowerBodyYawTarget, "DT_CSPlayer", "m_flLowerBodyYawTarget");
    NETVAR(int32_t,                          m_iHealth,              "DT_BasePlayer", "m_iHealth");
    NETVAR(int32_t,                          m_lifeState,            "DT_BasePlayer", "m_lifeState");
    NETVAR(int32_t,                          m_fFlags,               "DT_BasePlayer", "m_fFlags");
    NETVAR(int32_t,                          m_nTickBase,            "DT_BasePlayer", "m_nTickBase");
    NETVAR(Vector,                           m_vecViewOffset,        "DT_BasePlayer", "m_vecViewOffset[0]");
    NETVAR(QAngle,                           m_viewPunchAngle,       "DT_BasePlayer", "m_viewPunchAngle");
    NETVAR(QAngle,                           m_aimPunchAngle,        "DT_BasePlayer", "m_aimPunchAngle");
    NETVAR(CHandle<C_BaseViewModel>,         m_hViewModel,           "DT_BasePlayer", "m_hViewModel[0]");
    NETVAR(Vector,                           m_vecVelocity,          "DT_BasePlayer", "m_vecVelocity[0]");
    NETVAR(float,                            m_flMaxspeed,           "DT_BasePlayer", "m_flMaxspeed");
    NETVAR(CHandle<C_BasePlayer>,            m_hObserverTarget,      "DT_BasePlayer", "m_hObserverTarget");
	NETVAR(int,                              m_iObserverMode,        "DT_BasePlayer", "m_iObserverMode");
    NETVAR(CHandle<C_BaseCombatWeapon>,      m_hActiveWeapon,        "DT_BaseCombatCharacter", "m_hActiveWeapon");
    PNETVAR(CHandle<C_BaseCombatWeapon>,     m_hMyWeapons,           "DT_BaseCombatCharacter", "m_hMyWeapons");
    PNETVAR(CHandle<C_BaseAttributableItem>, m_hMyWearables,         "DT_BaseCombatCharacter", "m_hMyWearables");
	NETVAR(float, m_flFlashDuration, "DT_CSPlayer", "m_flFlashDuration");
	NETVAR(float, m_flSimulationTime, "DT_CSPlayer", "m_flSimulationTime");;

	int GetMoveType() {
		return *reinterpret_cast<int*>((uintptr_t)this + (uintptr_t)0x258);
	}
    
	void SetAngle2(const QAngle& wantedang)
	{
		typedef void(__thiscall* SetAngleFn)(void*, const QAngle&);
		static SetAngleFn SetAngle = (SetAngleFn)((DWORD)GetModuleHandleW(L"client.dll") + 0x1C2AC0);
		SetAngle(this, wantedang);
	}

    CUserCmd*& m_pCurrentCommand();

    Vector        GetEyePos();
    player_info_t GetPlayerInfo();
    bool          IsAlive();
    bool          HasC4();
    Vector        GetHitboxPos(int hitbox_id);
    bool          GetHitboxPos(int hitbox, Vector &output);
    Vector        GetBonePos(int bone);
    bool          CanSeePlayer(C_BasePlayer* player, int hitbox);
    bool          CanSeePlayer(C_BasePlayer* player, const Vector& pos);

	bool IsMoving()
	{
		if (this->m_vecVelocity().Length2D() > 0.0f)
			return true;

		return false;
	}

	bool IsBreakingLBY()
	{
		return IsMoving();
	}

	bool IsInFakewalk()
	{
		if (this->m_vecVelocity().Length2D() > 0.0f && this->m_vecVelocity().Length2D() <= 40.0f)
			return true;
		
		return false;
	}
};

class C_BaseViewModel : public C_BaseEntity
{
public:
    NETVAR(int32_t, m_nModelIndex, "DT_BaseViewModel", "m_nModelIndex");
    NETVAR(int32_t, m_nViewModelIndex, "DT_BaseViewModel", "m_nViewModelIndex");
    NETVAR(CHandle<C_BaseCombatWeapon>, m_hWeapon, "DT_BaseViewModel", "m_hWeapon");
    NETVAR(CHandle<C_BasePlayer>, m_hOwner, "DT_BaseViewModel", "m_hOwner");

	void SetWeaponModel(const char* Filename, IClientEntity* Weapon)
	{
		typedef void(__thiscall* tSetWeaponModel)(void*, const char*, IClientEntity*);
		return CallVFunction<tSetWeaponModel>(this, 242)(this, Filename, Weapon);
	}
};