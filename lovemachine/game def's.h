#pragma once
#include "vector.h"
#include "memory.h"

enum e_ls : int
{
	life_alive,
	life_dying,
	life_dead,
	life_respawnable,
	life_discardbody
};

enum e_hitboxes
{
	hitbox_pelvis = 0,
	hitbox_r_up_leg = 2,
	hitbox_r_low_leg = 3,
	hitbox_l_up_leg = 6,
	hitbox_l_low_leg = 7,
	hitbox_stomach = 10,
	hitbox_chest = 11,
	hitbox_upper_chest = 12,
	hitbox_neck = 13,
	hitbox_head = 14,
	hitbox_l_low_arm = 17,
	hitbox_l_up_arm = 18,
	hitbox_r_low_arm = 30,
	hitbox_r_up_arm = 31
};

enum clientframestage_t
{
	FRAME_UNDEFINED = -1,			// (haven't run any frames yet)
	FRAME_START,

	// A network packet is being recieved
	FRAME_NET_UPDATE_START,
	// Data has been received and we're going to start calling PostDataUpdate
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	// Data has been received and we've called PostDataUpdate on all data recipients
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	// We've received all packets, we can now do interpolation, prediction, etc..
	FRAME_NET_UPDATE_END,

	// We're about to start rendering the scene
	FRAME_RENDER_START,
	// We've finished rendering the scene.
	FRAME_RENDER_END
};

enum e_weapon
{
	weapon_none,
	weapon_p228,
	weapon_glock,
	weapon_scout,
	weapon_he_grenade,
	weapon_xm1014,
	weapon_c4,
	weapon_mac10,
	weapon_aug,
	weapon_smoke_grenade,
	weapon_elites,
	weapon_fiveseven,
	weapon_ump,
	weapon_sg550,
	weapon_galil,
	weapon_famas,
	weapon_usp,
	weapon_awp,
	weapon_mp5,
	weapon_m249,
	weapon_m3,
	weapon_m4a1,
	weapon_tmp,
	weapon_g3sg1,
	weapon_flashbang,
	weapon_deagle,
	weapon_sg552,
	weapon_ak47,
	weapon_knife,
	weapon_p90,
	weapon_max
};

enum movetype_t
{
	movetype_none = 0,
	movetype_isometric,
	movetype_walk,
	movetype_step,
	movetype_fly,
	movetype_flygravity,
	movetype_vphysics,
	movetype_push,
	movetype_noclip,
	movetype_ladder,
	movetype_observer,
	movetype_custom,
	movetype_last = movetype_custom,
	movetype_max_bits = 4
};

enum class_ids
{
	CTestTraceline = 156,
	CTEWorldDecal = 157,
	CTESpriteSpray = 154,
	CTESprite = 153,
	CTESparks = 152,
	CTESmoke = 151,
	CTEShowLine = 149,
	CTEProjectedDecal = 146,
	CTEPlayerDecal = 145,
	CTEPhysicsProp = 142,
	CTEParticleSystem = 141,
	CTEMuzzleFlash = 140,
	CTELargeFunnel = 138,
	CTEKillPlayerAttachments = 137,
	CTEImpact = 136,
	CTEGlowSprite = 135,
	CTEShatterSurface = 148,
	CTEFootprintDecal = 133,
	CTEFizz = 132,
	CTEExplosion = 130,
	CTEEnergySplash = 129,
	CTEEffectDispatch = 128,
	CTEDynamicLight = 127,
	CTEDecal = 125,
	CTEClientProjectile = 124,
	CTEBubbleTrail = 123,
	CTEBubbles = 122,
	CTEBSPDecal = 121,
	CTEBreakModel = 120,
	CTEBloodStream = 119,
	CTEBloodSprite = 118,
	CTEBeamSpline = 117,
	CTEBeamRingPoint = 116,
	CTEBeamRing = 115,
	CTEBeamPoints = 114,
	CTEBeamLaser = 113,
	CTEBeamFollow = 112,
	CTEBeamEnts = 111,
	CTEBeamEntPoint = 110,
	CTEBaseBeam = 109,
	CTEArmorRicochet = 108,
	CTEMetalSparks = 139,
	CSteamJet = 104,
	CSmokeStack = 99,
	DustTrail = 188,
	CFireTrail = 47,
	SporeTrail = 195,
	SporeExplosion = 194,
	RocketTrail = 192,
	SmokeTrail = 193,
	CPropVehicleDriveable = 90,
	ParticleSmokeGrenade = 191,
	CParticleFire = 73,
	MovieExplosion = 189,
	CTEGaussExplosion = 134,
	CEnvQuadraticBeam = 41,
	CEmbers = 34,
	CEnvWind = 45,
	CPrecipitation = 87,
	CBaseTempEntity = 17,
	CWeaponXM1014 = 186,
	CWeaponUSP = 185,
	CWeaponUMP45 = 184,
	CWeaponTMP = 183,
	CSmokeGrenade = 98,
	CWeaponSG552 = 182,
	CWeaponSG550 = 181,
	CWeaponScout = 180,
	CWeaponP90 = 179,
	CWeaponP228 = 178,
	CWeaponMP5Navy = 177,
	CWeaponMAC10 = 176,
	CWeaponM4A1 = 175,
	CWeaponM3 = 174,
	CWeaponM249 = 173,
	CKnife = 70,
	CHEGrenade = 65,
	CWeaponGlock = 172,
	CWeaponGalil = 171,
	CWeaponG3SG1 = 170,
	CFlashbang = 49,
	CWeaponFiveSeven = 169,
	CWeaponFamas = 168,
	CWeaponElite = 167,
	CDEagle = 31,
	CWeaponCSBaseGun = 165,
	CWeaponCSBase = 164,
	CC4 = 23,
	CBaseCSGrenade = 6,
	CWeaponAWP = 163,
	CWeaponAug = 162,
	CAK47 = 1,
	NextBotCombatCharacter = 190,
	CFootstepControl = 51,
	CCSGameRulesProxy = 26,
	CWeaponCubemap = 0,
	CWeaponCycler = 166,
	CTEPlantBomb = 143,
	CTEFireBullets = 131,
	CTERadioIcon = 147,
	CPlantedC4 = 81,
	CCSTeam = 30,
	CCSPlayerResource = 28,
	CCSPlayer = 27,
	CCSRagdoll = 29,
	CTEPlayerAnimEvent = 144,
	CHostage = 66,
	CBaseCSGrenadeProjectile = 7,
	CHandleTest = 64,
	CTeamplayRoundBasedRulesProxy = 107,
	CSpriteTrail = 103,
	CSpriteOriented = 102,
	CSprite = 101,
	CRagdollPropAttached = 93,
	CRagdollProp = 92,
	CPredictedViewModel = 88,
	CPoseController = 86,
	CGameRulesProxy = 63,
	CInfoLadderDismount = 67,
	CFuncLadder = 56,
	CEnvDetailController = 38,
	CWorld = 187,
	CWaterLODControl = 161,
	CWaterBullet = 160,
	CVoteController = 159,
	CVGuiScreen = 158,
	CPropJeep = 89,
	CPropVehicleChoreoGeneric = 0,
	CTest_ProxyToggle_Networkable = 155,
	CTesla = 150,
	CTeamTrainWatcher = 0,
	CBaseTeamObjectiveResource = 16,
	CTeam = 106,
	CSun = 105,
	CParticlePerformanceMonitor = 74,
	CSpotlightEnd = 100,
	CSlideshowDisplay = 97,
	CShadowControl = 96,
	CSceneEntity = 95,
	CRopeKeyframe = 94,
	CRagdollManager = 91,
	CPhysicsPropMultiplayer = 79,
	CPhysBoxMultiplayer = 77,
	CBasePropDoor = 15,
	CDynamicProp = 33,
	CPointCommentaryNode = 85,
	CPointCamera = 84,
	CPlayerResource = 83,
	CPlasma = 82,
	CPhysMagnet = 80,
	CPhysicsProp = 78,
	CPhysBox = 76,
	CParticleSystem = 75,
	CMaterialModifyControl = 72,
	CLightGlow = 71,
	CInfoOverlayAccessor = 69,
	CFuncTrackTrain = 62,
	CFuncSmokeVolume = 61,
	CFuncRotating = 60,
	CFuncReflectiveGlass = 59,
	CFuncOccluder = 58,
	CFuncMonitor = 57,
	CFunc_LOD = 53,
	CTEDust = 126,
	CFunc_Dust = 52,
	CFuncConveyor = 55,
	CBreakableSurface = 22,
	CFuncAreaPortalWindow = 54,
	CFish = 48,
	CEntityFlame = 36,
	CFireSmoke = 46,
	CEnvTonemapController = 44,
	CEnvScreenEffect = 42,
	CEnvScreenOverlay = 43,
	CEnvProjectedTexture = 40,
	CEnvParticleScript = 39,
	CFogController = 50,
	CEntityParticleTrail = 37,
	CEntityDissolve = 35,
	CDynamicLight = 32,
	CColorCorrectionVolume = 25,
	CColorCorrection = 24,
	CBreakableProp = 21,
	CBasePlayer = 13,
	CBaseFlex = 10,
	CBaseEntity = 9,
	CBaseDoor = 8,
	CBaseCombatCharacter = 4,
	CBaseAnimatingOverlay = 3,
	CBoneFollower = 20,
	CBaseAnimating = 2,
	CInfoLightingRelative = 68,
	CAI_BaseNPC = 0,
	CBeam = 19,
	CBaseViewModel = 18,
	CBaseProjectile = 14,
	CBaseParticleEntity = 12,
	CBaseGrenade = 11,
	CBaseCombatWeapon = 5
};

enum cursor_code
{
	dc_user,
	dc_none,
	dc_arrow,
	dc_ibeam,
	dc_hourglass,
	dc_waitarrow,
	dc_crosshair,
	dc_up,
	dc_sizenwse,
	dc_sizenesw,
	dc_sizewe,
	dc_sizens,
	dc_sizeall,
	dc_no,
	dc_hand,
	dc_blank, // don't show any custom vgui cursor, just let windows do it stuff (for HTML widget)
	dc_last,
};

// adds to the font
enum font_flags
{
	ff_none,
	ff_italic = 0x001,
	ff_underline = 0x002,
	ff_strikeout = 0x004,
	ff_symbol = 0x008,
	ff_antialias = 0x010,
	ff_gaussianblur = 0x020,
	ff_rotary = 0x040,
	ff_dropshadow = 0x080,
	ff_additive = 0x100,
	ff_outline = 0x200,
	ff_custom = 0x400,		// custom generated font - never fall back to asian compatibility mode
	ff_bitmap = 0x800,		// compiled bitmap font - no fallbacks
};

#define fw_dontcare         0
#define fw_thin             100
#define fw_extralight       200
#define fw_light            300
#define fw_normal           400
#define fw_medium           500
#define fw_semibold         600
#define fw_bold             700
#define fw_extrabold        800
#define fw_heavy            900

#define fw_ultralight       fw_extralight
#define fw_regular          fw_normal
#define fw_demibold         fw_semibold
#define fw_ultrabold        fw_extrabold
#define fw_black            fw_heavy

const char* weapon_names[weapon_max] =
{
	"none",
	"p228",
	"glock",
	"scout",
	"he_grenade",
	"xm014",
	"c4",
	"mac10",
	"aug",
	"smoke_grende",
	"elites",
	"fiveseven",
	"ump",
	"sg550",
	"galil",
	"famas",
	"usp",
	"awp",
	"mp5",
	"m249",
	"m3",
	"m4a1",
	"tmp",
	"g3sg1",
	"flashbang",
	"deagle",
	"sg552",
	"ak47",
	"knife",
	"p90"
};

enum weapon_type
{
	weapontype_knife = 0,
	weapontype_pistol,
	weapontype_submachinegun,
	weapontype_rifle,
	weapontype_shotgun,
	weapontype_sniper_rifle,
	weapontype_machinegun,
	weapontype_c4,
	weapontype_grenade,
	weapontype_unknown
};

typedef enum {
	EMPTY,
	SINGLE,
	SINGLE_NPC,
	WPN_DOUBLE, // Can't be "DOUBLE" because windows.h uses it.
	DOUBLE_NPC,
	BURST,
	RELOAD,
	RELOAD_NPC,
	MELEE_MISS,
	MELEE_HIT,
	MELEE_HIT_WORLD,
	SPECIAL1,
	SPECIAL2,
	SPECIAL3,

	// Add new shoot sound types here

	NUM_SHOOT_SOUND_TYPES,
} WeaponSound_t;

#define	MAX_AMMO_TYPES	32		// ???

// Handy defines for EmitSound
// iEntIndex
#define SOUND_FROM_LOCAL_PLAYER		-1
#define SOUND_FROM_WORLD			0

#define MAX_SHOOT_SOUNDS	16			// Maximum number of shoot sounds per shoot type

#define MAX_WEAPON_STRING	80
#define MAX_WEAPON_PREFIX	16
#define MAX_WEAPON_AMMO_NAME		32

#define MAX_WEAPON_SLOTS		6	// hud item selection slots
#define MAX_WEAPON_POSITIONS	20	// max number of items within a slot
#define MAX_ITEM_TYPES			6	// hud item selection slots
#define MAX_WEAPONS				48	// Max number of weapons available

#define WEAPON_PRINTNAME_MISSING "!!! Missing printname on weapon"

// ---------------------------
//  Hit Group standards
// ---------------------------
#define	hg_generic	0
#define	hg_head		1
#define	hg_chest	2
#define	hg_stomach	3
#define hg_leftarm	4	
#define hg_rightarm	5
#define hg_leftleg	6
#define hg_rightleg	7
#define hg_gear		10			// alerts NPC, but doesn't do damage or bleed (1/100th damage)

// These are the names of the ammo types that go in the CAmmoDefs and that the 
// weapon script files reference.
#define BULLET_PLAYER_50AE		"BULLET_PLAYER_50AE"
#define BULLET_PLAYER_762MM		"BULLET_PLAYER_762MM"
#define BULLET_PLAYER_556MM		"BULLET_PLAYER_556MM"
#define BULLET_PLAYER_556MM_BOX	"BULLET_PLAYER_556MM_BOX"
#define BULLET_PLAYER_338MAG	"BULLET_PLAYER_338MAG"
#define BULLET_PLAYER_9MM		"BULLET_PLAYER_9MM"
#define BULLET_PLAYER_BUCKSHOT	"BULLET_PLAYER_BUCKSHOT"
#define BULLET_PLAYER_45ACP		"BULLET_PLAYER_45ACP"
#define BULLET_PLAYER_357SIG	"BULLET_PLAYER_357SIG"
#define BULLET_PLAYER_57MM		"BULLET_PLAYER_57MM"
#define AMMO_TYPE_HEGRENADE		"AMMO_TYPE_HEGRENADE"
#define AMMO_TYPE_FLASHBANG		"AMMO_TYPE_FLASHBANG"
#define AMMO_TYPE_SMOKEGRENADE	"AMMO_TYPE_SMOKEGRENADE"

#define DMG_BULLET			(1 << 1)	// shot
#define DMG_BLAST			(1 << 6)	// explosive blast damage

#define BULLET_IMPULSE_EXAGGERATION			1	

#define	CONTENTS_GRATE			0x8		// alpha-tested "grate" textures.  

enum AmmoTracer_t
{
	TRACER_NONE,
	TRACER_LINE,
	TRACER_RAIL,
	TRACER_BEAM,
	TRACER_LINE_AND_WHIZ,
};

class chudtexture;
class key_value;
class clientclass;

class convar;

struct Ammo_t
{
	char* pName;
	int					nDamageType;
	int					eTracerType;
	float				physicsForceImpulse;
	int					nMinSplashSize;
	int					nMaxSplashSize;

	int					nFlags;

	// Values for player/NPC damage and carrying capability
	// If the integers are set, they override the CVars
	int					pPlrDmg;		// CVar for player damage amount
	int					pNPCDmg;		// CVar for NPC damage amount
	int					pMaxCarry;		// CVar for maximum number can carry
	const convar* pPlrDmgCVar;	// CVar for player damage amount
	const convar* pNPCDmgCVar;	// CVar for NPC damage amount
	const convar* pMaxCarryCVar;	// CVar for maximum number can carry
};

class fileweaponinfo_t
{
public:
	fileweaponinfo_t() { }

	virtual void Parse(key_value*, const char*) = 0;

	bool bParsedScript; //0x0004
	bool bLoadedHudElements; //0x0005
	char szClassName[MAX_WEAPON_STRING]; //0x0006
	char szPrintName[MAX_WEAPON_STRING]; //0x0056
	char szViewModel[MAX_WEAPON_STRING]; //0x00A6
	char szWorldModel[MAX_WEAPON_STRING]; //0x00F6
	char szAnimationPrefix[MAX_WEAPON_PREFIX]; //0x0146
	char pad_0156[2]; //0x0156
	int32_t iSlot; //0x0158
	int32_t iPosition; //0x015C
	int32_t iMaxClip1; //0x0160
	int32_t iMaxClip2; //0x0164
	int32_t iDefaultClip1; //0x0168
	int32_t iDefaultClip2; //0x016C
	int32_t iWeight; //0x0170
	int32_t iRumbleEffect; //0x0174
	bool bAutoSwitchTo; //0x0178
	bool bAutoSwitchFrom; //0x0179
	char pad_017A[2]; //0x017A
	int32_t iFlags; //0x017C
	char szAmmo1[MAX_WEAPON_AMMO_NAME]; //0x0180
	char szAmmo2[MAX_WEAPON_AMMO_NAME]; //0x01A0
	char aShootSounds[NUM_SHOOT_SOUND_TYPES][MAX_WEAPON_STRING]; //0x01C0
	int32_t iAmmoType; //0x06C0
	int32_t iAmmo2Type; //0x06C4
	bool m_bMeleeWeapon; //0x06C8
	bool m_bBuiltRightHanded; //0x06C9
	bool m_bAllowFlipping; //0x06CA
	char pad_06CB[1]; //0x06CB
	int32_t iSpriteCount; //0x06CC
	class CHudTexture* iconActive; //0x06D0
	class CHudTexture* iconInactive; //0x06D4
	class CHudTexture* iconAmmo; //0x06D8
	class CHudTexture* iconAmmo2; //0x06DC
	class CHudTexture* iconCrosshair; //0x06E0
	class CHudTexture* iconAutoaim; //0x06E4
	class CHudTexture* iconZoomedCrosshair; //0x06E8
	class CHudTexture* iconZoomedAutoaim; //0x06EC
	class CHudTexture* iconSmall; //0x06F0
	bool bShowUsageHint; //0x06F4
	char pad_06F5[3]; //0x06F5
	float m_flMaxSpeed; //0x06F8
	weapon_type m_WeaponType; //0x06FC
	bool bFullAuto; //0x0700
	char pad_0701[3]; //0x0701
	int32_t m_iTeam; //0x0704
	float m_flBotAudibleRange; //0x0708
	float m_flArmorRatio; //0x070C
	int32_t m_iCrosshairMinDistance; //0x0710
	int32_t m_iCrosshairDeltaDistance; //0x0714
	bool m_bCanUseWithShield; //0x0718
	char m_WrongTeamMsg[32]; //0x0719
	char m_szAnimExtension[16]; //0x0739
	char m_szShieldViewModel[64]; //0x0749
	char m_szAddonModel[MAX_WEAPON_STRING]; //0x0789
	char m_szDroppedModel[MAX_WEAPON_STRING]; //0x07D9
	char m_szSilencerModel[MAX_WEAPON_STRING]; //0x0829
	char pad_0879[3]; //0x0879
	int32_t m_iMuzzleFlashStyle; //0x087C
	float m_flMuzzleScale; //0x0880
	int32_t m_iPenetration; //0x0884
	int32_t m_iDamage; //0x0888
	float m_flRange; //0x088C
	float m_flRangeModifier; //0x0890
	int32_t m_iBullets; //0x0894
	float m_flCycleTime; //0x0898
	bool m_bAccuracyQuadratic; //0x089C
	char pad_089D[3]; //0x089D
	float m_flAccuracyDivisor; //0x08A0
	float m_flAccuracyOffset; //0x08A4
	float m_flMaxInaccuracy; //0x08A8
	float m_flSpread; //0x08AC
	float m_flSpreadAlt; //0x08B0
	float m_flInaccuracyCrouch; //0x08B4
	float m_flInaccuracyCrouchAlt; //0x08B8
	float m_flInaccuracyStand; //0x08BC
	float m_flInaccuracyStandAlt; //0x08C0
	float m_flInaccuracyJump; //0x08C4
	float m_flInaccuracyJumpAlt; //0x08C8
	float m_flInaccuracyLand; //0x08CC
	float m_flInaccuracyLandAlt; //0x08D0
	float m_flInaccuracyLadder; //0x08D4
	float m_flInaccuracyLadderAlt; //0x08D8
	float m_flInaccuracyFire; //0x08DC
	float m_flInaccuracyFireAlt; //0x08E0
	float m_flInaccuracyMove; //0x08E4
	float m_flInaccuracyMoveAlt; //0x08E8
	float m_flRecoveryTimeStand; //0x08EC
	float m_flRecoveryTimeCrouch; //0x08F0
	float m_flInaccuracyReload; //0x08F4
	float m_flInaccuracyAltSwitch; //0x08F8
	float m_flTimeToIdleAfterFire; //0x08FC
	float m_flIdleInterval; //0x0900
	int32_t m_iWeaponPrice; //0x0904
	int32_t m_iDefaultPrice; //0x0908
	int32_t m_iPreviousPrice; //0x090C
}; //Size: 0x0910

//AUTOWALL: фикс ошибок
//=============================================================================
//	>> CAmmoDef
//=============================================================================
/*class CAmmoDef
{

public:
	int					m_nAmmoIndex;

	Ammo_t				m_AmmoType[MAX_AMMO_TYPES];

	Ammo_t* GetAmmoOfIndex(int nAmmoIndex);
	int					Index(const char* psz);
	int					PlrDamage(int nAmmoIndex);
	int					NPCDamage(int nAmmoIndex);
	int					MaxCarry(int nAmmoIndex);
	int					DamageType(int nAmmoIndex);
	int					TracerType(int nAmmoIndex);
	float				DamageForce(int nAmmoIndex);
	int					MinSplashSize(int nAmmoIndex);
	int					MaxSplashSize(int nAmmoIndex);
	int					Flags(int nAmmoIndex);

	void				AddAmmoType(char const* name, int damageType, int tracerType, int plr_dmg, int npc_dmg, int carry, float physicsForceImpulse, int nFlags, int minSplashSize = 4, int maxSplashSize = 8);
	void				AddAmmoType(char const* name, int damageType, int tracerType, char const* plr_cvar, char const* npc_var, char const* carry_cvar, float physicsForceImpulse, int nFlags, int minSplashSize = 4, int maxSplashSize = 8);

	CAmmoDef(void);
	virtual ~CAmmoDef(void);

private:
	bool				AddAmmoType(char const* name, int damageType, int tracerType, int nFlags, int minSplashSize, int maxSplashSize);
};

struct CSAmmoCost
{
	int	buySize;
	int	cost;
};

//=============================================================================
//	>> CCSAmmoDef
//=============================================================================
class CCSAmmoDef : public CAmmoDef
{

public:

	void AddAmmoCost(char const* name, int cost, int buySize);

	CCSAmmoDef(void);
	~CCSAmmoDef(void);

	int GetBuySize(int nAmmoIndex) const;
	int GetCost(int nAmmoIndex) const;

private:
	CSAmmoCost	m_csAmmo[MAX_AMMO_TYPES];
};

static CCSAmmoDef ammoDef;
CAmmoDef* GetAmmoDef()
{
	static bool bInitted = false;

	if (!bInitted)
	{
		bInitted = true;

		ammoDef.AddAmmoType(BULLET_PLAYER_50AE, DMG_BULLET, TRACER_LINE, 0, 0, "ammo_50AE_max", 2400 * BULLET_IMPULSE_EXAGGERATION, 0, 10, 14);
		ammoDef.AddAmmoType(BULLET_PLAYER_762MM, DMG_BULLET, TRACER_LINE, 0, 0, "ammo_762mm_max", 2400 * BULLET_IMPULSE_EXAGGERATION, 0, 10, 14);
		ammoDef.AddAmmoType(BULLET_PLAYER_556MM, DMG_BULLET, TRACER_LINE, 0, 0, "ammo_556mm_max", 2400 * BULLET_IMPULSE_EXAGGERATION, 0, 10, 14);
		ammoDef.AddAmmoType(BULLET_PLAYER_556MM_BOX, DMG_BULLET, TRACER_LINE, 0, 0, "ammo_556mm_box_max", 2400 * BULLET_IMPULSE_EXAGGERATION, 0, 10, 14);
		ammoDef.AddAmmoType(BULLET_PLAYER_338MAG, DMG_BULLET, TRACER_LINE, 0, 0, "ammo_338mag_max", 2800 * BULLET_IMPULSE_EXAGGERATION, 0, 12, 16);
		ammoDef.AddAmmoType(BULLET_PLAYER_9MM, DMG_BULLET, TRACER_LINE, 0, 0, "ammo_9mm_max", 2000 * BULLET_IMPULSE_EXAGGERATION, 0, 5, 10);
		ammoDef.AddAmmoType(BULLET_PLAYER_BUCKSHOT, DMG_BULLET, TRACER_LINE, 0, 0, "ammo_buckshot_max", 600 * BULLET_IMPULSE_EXAGGERATION, 0, 3, 6);
		ammoDef.AddAmmoType(BULLET_PLAYER_45ACP, DMG_BULLET, TRACER_LINE, 0, 0, "ammo_45acp_max", 2100 * BULLET_IMPULSE_EXAGGERATION, 0, 6, 10);
		ammoDef.AddAmmoType(BULLET_PLAYER_357SIG, DMG_BULLET, TRACER_LINE, 0, 0, "ammo_357sig_max", 2000 * BULLET_IMPULSE_EXAGGERATION, 0, 4, 8);
		ammoDef.AddAmmoType(BULLET_PLAYER_57MM, DMG_BULLET, TRACER_LINE, 0, 0, "ammo_57mm_max", 2000 * BULLET_IMPULSE_EXAGGERATION, 0, 4, 8);
		ammoDef.AddAmmoType(AMMO_TYPE_HEGRENADE, DMG_BLAST, TRACER_LINE, 0, 0, 1, 1, 0);
		ammoDef.AddAmmoType(AMMO_TYPE_FLASHBANG, 0, TRACER_LINE, 0, 0, 2, 1, 0);
		ammoDef.AddAmmoType(AMMO_TYPE_SMOKEGRENADE, 0, TRACER_LINE, 0, 0, 1, 1, 0);

		//Adrian: I set all the prices to 0 just so the rest of the buy code works
		//This should be revisited.
		ammoDef.AddAmmoCost(BULLET_PLAYER_50AE, 0, 7);
		ammoDef.AddAmmoCost(BULLET_PLAYER_762MM, 0, 30);
		ammoDef.AddAmmoCost(BULLET_PLAYER_556MM, 0, 30);
		ammoDef.AddAmmoCost(BULLET_PLAYER_556MM_BOX, 0, 30);
		ammoDef.AddAmmoCost(BULLET_PLAYER_338MAG, 0, 10);
		ammoDef.AddAmmoCost(BULLET_PLAYER_9MM, 0, 30);
		ammoDef.AddAmmoCost(BULLET_PLAYER_BUCKSHOT, 0, 8);
		ammoDef.AddAmmoCost(BULLET_PLAYER_45ACP, 0, 25);
		ammoDef.AddAmmoCost(BULLET_PLAYER_357SIG, 0, 13);
		ammoDef.AddAmmoCost(BULLET_PLAYER_57MM, 0, 50);
	}

	return &ammoDef;
}

bool IsAmmoType(int iAmmoType, const char* pAmmoName)
{
	return GetAmmoDef()->Index(pAmmoName) == iAmmoType;
}*/

class iclientnetworkable
{
public:
	clientclass* get_clientclass()
	{
		typedef clientclass*(__thiscall* get_clientclass_fn)(void*);
		return vfunc< get_clientclass_fn >(this, 2)(this);
	}

	bool is_dormant()
	{
		typedef bool(__thiscall* is_dormant_fn)(void*);
		return vfunc< is_dormant_fn >(this, 8)(this);
	}
};

#define MAX_EVENT_NAME_LENGTH	32		// max game event name length
#define MAX_EVENT_BITS			9		// max bits needed for an event index
#define MAX_EVENT_NUMBER		(1<<MAX_EVENT_BITS)		// max number of events allowed
#define MAX_EVENT_BYTES			1024	// max size in bytes for a serialized event

class igameevent
{
public:
	virtual ~igameevent() {};
	virtual const char* get_name() const = 0;	// get event name

	virtual bool  is_reliable() const = 0; // if event handled reliable
	virtual bool  is_local() const = 0; // if event is never networked
	virtual bool  is_empty(const char* keyName = NULL) = 0; // check if data field exists

	// Data access
	virtual bool  get_bool(const char* keyName = NULL, bool defaultValue = false) = 0;
	virtual int   get_int(const char* keyName = NULL, int defaultValue = 0) = 0;
	virtual float get_float(const char* keyName = NULL, float defaultValue = 0.0f) = 0;
	virtual const char* get_string(const char* keyName = NULL, const char* defaultValue = "") = 0;

	virtual void set_bool(const char* keyName, bool value) = 0;
	virtual void set_int(const char* keyName, int value) = 0;
	virtual void set_float(const char* keyName, float value) = 0;
	virtual void set_string(const char* keyName, const char* value) = 0;
};

struct ray_t;
class CGameTrace;
typedef CGameTrace trace_t;
struct model_t;
struct matrix3x4_t;

class icollideable
{
public:
	// Gets at the entity handle associated with the collideable
	virtual void* GetEntityHandle() = 0;

	// These methods return the bounds of an OBB measured in "collision" space
	// which can be retreived through the CollisionToWorldTransform or
	// GetCollisionOrigin/GetCollisionAngles methods
	virtual const Vector& obb_mins() const = 0;
	virtual const Vector& obb_maxs() const = 0;

	virtual void			func0(Vector* pVecWorldMins, Vector* pVecWorldMaxs) const = 0;
	virtual bool			func1(const ray_t& ray, unsigned int fContentsMask, trace_t& tr) = 0;
	virtual bool			func2(const ray_t& ray, unsigned int fContentsMask, trace_t& tr) = 0;
	virtual int				func3() = 0;
	virtual const model_t* func4() = 0;
	virtual const Vector& func5() const = 0;
	virtual const qangle& func6() const = 0;

	virtual const matrix3x4_t& collision_to_world_transform() const = 0;
};

//-----------------------------------------------------------------------------
// ConVar flags
//-----------------------------------------------------------------------------
// The default, no flags at all
#define FCVAR_NONE				0 

// Command to ConVars and ConCommands
// ConVar Systems
#define FCVAR_UNREGISTERED		(1<<0)	// If this is set, don't add to linked list, etc.
#define FCVAR_DEVELOPMENTONLY	(1<<1)	// Hidden in released products. Flag is removed automatically if ALLOW_DEVELOPMENT_CVARS is defined.
#define FCVAR_GAMEDLL			(1<<2)	// defined by the game DLL
#define FCVAR_CLIENTDLL			(1<<3)  // defined by the client DLL
#define FCVAR_HIDDEN			(1<<4)	// Hidden. Doesn't appear in find or autocomplete. Like DEVELOPMENTONLY, but can't be compiled out.

// ConVar only
#define FCVAR_PROTECTED			(1<<5)  // It's a server cvar, but we don't send the data since it's a password, etc.  Sends 1 if it's not bland/zero, 0 otherwise as value
#define FCVAR_SPONLY			(1<<6)  // This cvar cannot be changed by clients connected to a multiplayer server.
#define	FCVAR_ARCHIVE			(1<<7)	// set to cause it to be saved to vars.rc
#define	FCVAR_NOTIFY			(1<<8)	// notifies players when changed
#define	FCVAR_USERINFO			(1<<9)	// changes the client's info string
#define FCVAR_CHEAT				(1<<14) // Only useable in singleplayer / debug / multiplayer & sv_cheats

#define FCVAR_PRINTABLEONLY		(1<<10)  // This cvar's string cannot contain unprintable characters ( e.g., used for player name etc ).
#define FCVAR_UNLOGGED			(1<<11)  // If this is a FCVAR_SERVER, don't log changes to the log file / console if we are creating a log
#define FCVAR_NEVER_AS_STRING	(1<<12)  // never try to print that cvar

// It's a ConVar that's shared between the client and the server.
// At signon, the values of all such ConVars are sent from the server to the client (skipped for local
//  client, of course )
// If a change is requested it must come from the console (i.e., no remote client changes)
// If a value is changed while a server is active, it's replicated to all connected clients
#define FCVAR_REPLICATED		(1<<13)	// server setting enforced on clients, TODO rename to FCAR_SERVER at some time
#define FCVAR_DEMO				(1<<16)  // record this cvar when starting a demo file
#define FCVAR_DONTRECORD		(1<<17)  // don't record these command in demofiles

#define FCVAR_NOT_CONNECTED		(1<<22)	// cvar cannot be changed by a client that is connected to a server

#define FCVAR_ARCHIVE_XBOX		(1<<24) // cvar written to config.cfg on the Xbox

#define FCVAR_SERVER_CAN_EXECUTE	(1<<28)// the server is allowed to execute this command on clients via ClientCommand/NET_StringCmd/CBaseClientState::ProcessStringCmd.
#define FCVAR_SERVER_CANNOT_QUERY	(1<<29)// If this is set, then the server is not allowed to query this cvar's value (via IServerPluginHelpers::StartQueryCvarValue).
#define FCVAR_CLIENTCMD_CAN_EXECUTE	(1<<30)	// IVEngineClient::ClientCmd is allowed to execute this command. 

class iconvar
{
public:
	// Value set
	virtual void SetValue(const char* pValue) = 0;
	virtual void SetValue(float flValue) = 0;
	virtual void SetValue(int nValue) = 0;

	// Return name of command
	virtual const char* GetName(void) const = 0;

	// Accessors.. not as efficient as using GetState()/GetInfo()
	// if you call these methods multiple times on the same IConVar
	virtual bool IsFlagSet(int nFlag) const = 0;
};

typedef void (*FnChangeCallback_t)(iconvar* var, const char* pOldValue, float flOldValue);

class convar : public iconvar
{
public:
	virtual						~convar(void);

	virtual bool				IsFlagSet(int flag) const;
	virtual const char* GetHelpText(void) const;
	virtual bool				IsRegistered(void) const;
	virtual const char* GetName(void) const;
	virtual void				AddFlags(int flags);
	virtual	bool				IsCommand(void) const;

	// Any function that allocates/frees memory needs to be virtual or else you'll have crashes
	//  from alloc/free across dll/exe boundaries.

	// These just call into the IConCommandBaseAccessor to check flags and set the var (which ends up calling InternalSetValue).
	virtual void				SetValue(const char* value);
	virtual void				SetValue(float value);
	virtual void				SetValue(int value);

	const char* GetDefault(void) const;

private:
	// Called by CCvar when the value of a var is changing.
	virtual void				InternalSetValue(const char* value);
	// For CVARs marked FCVAR_NEVER_AS_STRING
	virtual void				InternalSetFloatValue(float fNewValue);
	virtual void				InternalSetIntValue(int nValue);

	virtual bool				ClampValue(float& value);
	virtual void				ChangeStringValue(const char* tempVal, float flOldValue);

	virtual void				Create(const char* pName, const char* pDefaultValue, int flags = 0,
		const char* pHelpString = 0, bool bMin = false, float fMin = 0.0,
		bool bMax = false, float fMax = false, FnChangeCallback_t callback = 0);

	// Used internally by OneTimeInit to initialize.
	virtual void				Init();

public:

	// This either points to "this" or it points to the original declaration of a ConVar.
	// This allows ConVars to exist in separate modules, and they all use the first one to be declared.
	// m_pParent->m_pParent must equal m_pParent (ie: m_pParent must be the root, or original, ConVar).
	convar* m_pParent;

	// Static data
	const char* m_pszDefaultValue;

	// Value
	// Dynamically allocated
	char* m_pszString;
	int							m_StringLength;

	// Values
	float						m_fValue;
	int							m_nValue;

	// Min/Max values
	bool						m_bHasMin;
	float						m_fMinVal;
	bool						m_bHasMax;
	float						m_fMaxVal;

	// Call this function when ConVar changes
	FnChangeCallback_t			m_fnChangeCallback;
};

enum OverrideType_t
{
	OVERRIDE_NORMAL = 0,
	OVERRIDE_BUILD_SHADOWS,
	OVERRIDE_DEPTH_WRITE,
};

typedef unsigned short materialhandle_t;

enum mat_var_flags
{
	MATERIAL_VAR_DEBUG = (1 << 0),
	MATERIAL_VAR_NO_DEBUG_OVERRIDE = (1 << 1),
	MATERIAL_VAR_NO_DRAW = (1 << 2),
	MATERIAL_VAR_USE_IN_FILLRATE_MODE = (1 << 3),

	MATERIAL_VAR_VERTEXCOLOR = (1 << 4),
	MATERIAL_VAR_VERTEXALPHA = (1 << 5),
	MATERIAL_VAR_SELFILLUM = (1 << 6),
	MATERIAL_VAR_ADDITIVE = (1 << 7),
	MATERIAL_VAR_ALPHATEST = (1 << 8),
	MATERIAL_VAR_MULTIPASS = (1 << 9),
	MATERIAL_VAR_ZNEARER = (1 << 10),
	MATERIAL_VAR_MODEL = (1 << 11),
	MATERIAL_VAR_FLAT = (1 << 12),
	MATERIAL_VAR_NOCULL = (1 << 13),
	MATERIAL_VAR_NOFOG = (1 << 14),
	MATERIAL_VAR_IGNOREZ = (1 << 15),
	MATERIAL_VAR_DECAL = (1 << 16),
	MATERIAL_VAR_ENVMAPSPHERE = (1 << 17),
	MATERIAL_VAR_NOALPHAMOD = (1 << 18),
	MATERIAL_VAR_ENVMAPCAMERASPACE = (1 << 19),
	MATERIAL_VAR_BASEALPHAENVMAPMASK = (1 << 20),
	MATERIAL_VAR_TRANSLUCENT = (1 << 21),
	MATERIAL_VAR_NORMALMAPALPHAENVMAPMASK = (1 << 22),
	MATERIAL_VAR_NEEDS_SOFTWARE_SKINNING = (1 << 23),
	MATERIAL_VAR_OPAQUETEXTURE = (1 << 24),
	MATERIAL_VAR_ENVMAPMODE = (1 << 25),
	MATERIAL_VAR_SUPPRESS_DECALS = (1 << 26),
	MATERIAL_VAR_HALFLAMBERT = (1 << 27),
	MATERIAL_VAR_WIREFRAME = (1 << 28),
	MATERIAL_VAR_ALLOWALPHATOCOVERAGE = (1 << 29),

	// NOTE: Only add flags here that either should be read from
	// .vmts or can be set directly from client code. Other, internal
	// flags should to into the flag enum in IMaterialInternal.h
};

#define TEXTURE_GROUP_LIGHTMAP						"Lightmaps"
#define TEXTURE_GROUP_WORLD							"World textures"
#define TEXTURE_GROUP_MODEL							"Model textures"
#define TEXTURE_GROUP_VGUI							"VGUI textures"
#define TEXTURE_GROUP_PARTICLE						"Particle textures"
#define TEXTURE_GROUP_DECAL							"Decal textures"
#define TEXTURE_GROUP_SKYBOX						"SkyBox textures"
#define TEXTURE_GROUP_CLIENT_EFFECTS				"ClientEffect textures"
#define TEXTURE_GROUP_OTHER							"Other textures"
#define TEXTURE_GROUP_PRECACHED						"Precached"				// TODO: assign texture groups to the precached materials
#define TEXTURE_GROUP_CUBE_MAP						"CubeMap textures"
#define TEXTURE_GROUP_RENDER_TARGET					"RenderTargets"
#define TEXTURE_GROUP_UNACCOUNTED					"Unaccounted textures"	// Textures that weren't assigned a texture group.
//#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER		"Static Vertex"
#define TEXTURE_GROUP_STATIC_INDEX_BUFFER			"Static Indices"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_DISP		"Displacement Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_COLOR	"Lighting Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_WORLD	"World Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_MODELS	"Model Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_OTHER	"Other Verts"
#define TEXTURE_GROUP_DYNAMIC_INDEX_BUFFER			"Dynamic Indices"
#define TEXTURE_GROUP_DYNAMIC_VERTEX_BUFFER			"Dynamic Verts"
#define TEXTURE_GROUP_DEPTH_BUFFER					"DepthBuffer"
#define TEXTURE_GROUP_VIEW_MODEL					"ViewModel"
#define TEXTURE_GROUP_PIXEL_SHADERS					"Pixel Shaders"
#define TEXTURE_GROUP_VERTEX_SHADERS				"Vertex Shaders"
#define TEXTURE_GROUP_RENDER_TARGET_SURFACE			"RenderTarget Surfaces"
#define TEXTURE_GROUP_MORPH_TARGETS					"Morph Targets"

typedef float matrix3x4[3][4];
typedef float matrix4x4[4][4];

typedef fileweaponinfo_t& (__thiscall* get_data_fn)();
/*typedef void(__thiscall* get_bullet_type_fn)(centity*, int, float*, float*);

#define	CONTENTS_SOLID			0x1
#define	CONTENTS_WINDOW			0x2
#define	CONTENTS_GRATE			0x8
#define CONTENTS_MOVEABLE		0x4000
#define	CONTENTS_MONSTER		0x2000000
#define	MASK_SOLID					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)*/

//iwebz kolo (c)
//USERCMD OFFSETS
#define USERCMDOFFSET 0xC4
#define VERIFIEDCMDOFFSET 0xC8
#define MULTIPLAYER_BACKUP 90
#define CURRENTCOMMANDOFFSET 0x100C
#define CURRENTPLAYERCOMMANDOFFSET 0x1640
#define PREDICTIONSEEDOFFSET 0x1A
#define PREDICTIONPLAYEROFFSET 0x2D
#define GLOBALSOFFSET 0x34
#define INPUTOFFSET 0x20
#define GETSPREADOFFSET 0x5CC
#define GETCONEOFFSET 0x5D0
#define UPDATEACCURACYPENALTYOFFSET 0x5D4
#define WEAPONIDOFFSET 0x5A0
#define APPSYSTEMFACTORYOFFSET 0x2A
#define CLIENTFACTORYOFFSET 0x67

//USERCMD BUTTONS
#define IN_ATTACK		(1 << 0)
#define IN_JUMP			(1 << 1)
#define IN_DUCK			(1 << 2)
#define IN_FORWARD		(1 << 3)
#define IN_BACK			(1 << 4)
#define IN_USE			(1 << 5)
#define IN_CANCEL		(1 << 6)
#define IN_LEFT			(1 << 7)
#define IN_RIGHT		(1 << 8)
#define IN_MOVELEFT		(1 << 9)
#define IN_MOVERIGHT	(1 << 10)
#define IN_ATTACK2		(1 << 11)
#define IN_RUN			(1 << 12)
#define IN_RELOAD		(1 << 13)
#define IN_ALT1			(1 << 14)
#define IN_ALT2			(1 << 15)
#define IN_SCORE		(1 << 16)   // Used by client.dll for when scoreboard is held down
#define IN_SPEED		(1 << 17)	// Player is holding the speed key
#define IN_WALK			(1 << 18)	// Player holding walk key
#define IN_ZOOM			(1 << 19)	// Zoom key for HUD zoom
#define IN_WEAPON1		(1 << 20)	// weapon defines these bits
#define IN_WEAPON2		(1 << 21)	// weapon defines these bits
#define IN_BULLRUSH		(1 << 22)
#define IN_GRENADE1		(1 << 23)	// grenade 1
#define IN_GRENADE2		(1 << 24)	// grenade 2

//Player flags
#define	FL_ONGROUND				(1<<0)	// At rest / on the ground
#define FL_DUCKING				(1<<1)	// Player flag -- Player is fully crouched
#define	FL_WATERJUMP			(1<<3)	// player jumping out of water
#define FL_ONTRAIN				(1<<4) // Player is _controlling_ a train, so movement commands should be ignored on client during prediction.
#define FL_INRAIN				(1<<5)	// Indicates the entity is standing in rain
#define FL_FROZEN				(1<<6) // Player is frozen for 3rd person camera
#define FL_ATCONTROLS			(1<<7) // Player can't move, but keeps key inputs for controlling another entity
#define	FL_CLIENT				(1<<8)	// Is a player
#define FL_FAKECLIENT			(1<<9)	// Fake client, simulated server side; don't send network messages to them
#define	FL_INWATER				(1<<10)	// In water
//
//iwebz kolo (c)

struct matrix3x4_t
{
	matrix3x4_t() {}
	matrix3x4_t(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23)
	{
		m_flMatVal[0][0] = m00;	m_flMatVal[0][1] = m01; m_flMatVal[0][2] = m02; m_flMatVal[0][3] = m03;
		m_flMatVal[1][0] = m10;	m_flMatVal[1][1] = m11; m_flMatVal[1][2] = m12; m_flMatVal[1][3] = m13;
		m_flMatVal[2][0] = m20;	m_flMatVal[2][1] = m21; m_flMatVal[2][2] = m22; m_flMatVal[2][3] = m23;
	}
	void Init(const Vector& xAxis, const Vector& yAxis, const Vector& zAxis, const Vector& vecOrigin)
	{
		m_flMatVal[0][0] = xAxis.x; m_flMatVal[0][1] = yAxis.x; m_flMatVal[0][2] = zAxis.x; m_flMatVal[0][3] = vecOrigin.x;
		m_flMatVal[1][0] = xAxis.y; m_flMatVal[1][1] = yAxis.y; m_flMatVal[1][2] = zAxis.y; m_flMatVal[1][3] = vecOrigin.y;
		m_flMatVal[2][0] = xAxis.z; m_flMatVal[2][1] = yAxis.z; m_flMatVal[2][2] = zAxis.z; m_flMatVal[2][3] = vecOrigin.z;
	}

	matrix3x4_t(const Vector& xAxis, const Vector& yAxis, const Vector& zAxis, const Vector& vecOrigin)
	{
		Init(xAxis, yAxis, zAxis, vecOrigin);
	}
	float* operator[](int i)
	{
		return m_flMatVal[i];
	}
	const float* operator[](int i) const
	{
		return m_flMatVal[i];
	}
	float* Base()
	{
		return &m_flMatVal[0][0];
	}
	const float* Base() const
	{
		return &m_flMatVal[0][0];
	}

	float m_flMatVal[3][4];
};