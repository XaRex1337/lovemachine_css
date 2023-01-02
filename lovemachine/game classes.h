#pragma once
#include "global.h"
//#include "console.h"
#include "offsets.h"
#include "game def's.h"
#include "game shit.h"
#include "checksum_crc.h"

class cweapon; // forward declaration meme

class centity
{
public:
	// (c) interwebz css kolo <3
	int get_id()
	{
		int id;

		PVOID entity = (PVOID)this;

		__asm
		{
			MOV ECX, entity
			MOV EAX, DWORD PTR DS : [ECX + 0x8]
			MOV EDX, DWORD PTR DS : [EAX + 0x24]
			LEA ESI, DWORD PTR DS : [ECX + 0x8]
			MOV ECX, ESI
			CALL EDX
			MOV id, EAX
		}

		return id;
	}

	int get_hp()
	{
		return *(int*)((dword)this + offsets::health);
	}

	int get_armor()
	{
		return *(int*)((dword)this + offsets::armor);
	}

	// GDPR_Anonymous uc <3
	bool is_armored(int hitgroup)
	{
		if (get_armor() > 0)
		{
			switch (hitgroup)
			{
			case hg_generic:
			case hg_chest:
			case hg_stomach:
			case hg_leftarm:
			case hg_rightarm:
				return true;
				break;
			case hg_head:
				// TODO: найти как определить наличие шлемака
				if (true)//if (bHasHelmet())
				{
					return true;
				}
				break;
			default:
				break;
			}
		}

		return false;
	}

	int get_team()
	{
		return *(int*)((dword)this + offsets::team);
	}

	int get_flags()
	{
		return *(int*)((dword)this + offsets::flags);
	}
	
	void set_flags(int flags)
	{
		*(int*)((dword)this + offsets::flags) = flags;
	}

	//void get_bullet_type(int a1, float* a2, float* a3)
	//{
	//	game::signatures::get_bullet_type(this, a1, a2, a3);
	//}

	int get_tickbase()
	{
		return *(int*)((dword)this + offsets::tickbase);
	}

	int get_money()
	{
		return *(int*)((dword)this + offsets::account);
	}

	int get_shots()
	{
		return *(int*)((dword)this + offsets::shots_fired);
	}
	
	char get_life_state() // e_ls
	{
		return *(char*)((dword)this + offsets::life_state);
	}

	float get_c4_blow_time()
	{
		return *(float*)((dword)this + offsets::c4_blow);
	}

	float get_c4_timer_length()
	{
		return *(float*)((dword)this + offsets::c4_timer);
	}

	/*float get_c4_defuse_time()
	{
		return *(char*)((dword)this + offsets::c4_defuse);
	}*/

	float get_alpha()
	{
		auto model = this->get_model();
		if (!model) return -1.f;

		static imaterial* material[32]; _model_info->get_model_materials(model, 1, material);
		if (material[0])
			return material[0]->get_alpha_modulation();

		return -1.f;
	}

	// (c) interwebz css kolo <3
	// ток че то не работает ниху€
	bool is_dormant()
	{
		/*return true;
		bool dormant;

		PVOID entity = (PVOID)this;

		__asm
		{
			MOV ECX, entity
			MOV EAX, DWORD PTR DS : [ECX + 0x8]
			MOV EDX, DWORD PTR DS : [EAX + 0x20]
			LEA ESI, DWORD PTR DS : [ECX + 0x8]
			MOV ECX, ESI
			CALL EDX
			MOV dormant, AL
		}

		return dormant;*/

		const auto networkable = this->get_clientnetworkable();
		if (!networkable) return true;

		return networkable->is_dormant();
	}

	int get_class_id()
	{
		const auto networkable = this->get_clientnetworkable();
		if (!networkable) return -1;

		const auto client_class = networkable->get_clientclass();
		if (!client_class) return -1;

		return client_class->class_id;
	}

	bool valid()
	{
		if (this && this->get_hp() > 0 && (int)this->get_life_state() == life_alive && (this->get_team() == 2 || this->get_team() == 3) /*&& (this->get_id() == global::local_id || !this->is_dormant())*/)
			return true;

		return false;
	}

	bool have_defuser()
	{
		return *(bool*)((dword)this + offsets::have_defuser);
	}

	cvector get_origin()
	{
		return *(cvector*)((dword)this + offsets::origin);
	}

	void set_origin(cvector origin)
	{
		*(cvector*)((dword)this + offsets::origin) = origin;
	}

	cvector get_view_offset()
	{
		return *(cvector*)((dword)this + offsets::view_offset);
	}

	cvector get_eye_pos()
	{
		return (get_origin() + get_view_offset());
	}

	cvector get_velocity()
	{
		return *(cvector*)((dword)this + offsets::velocity);
	}

	void set_velocity(cvector velocity)
	{
		*(cvector*)((dword)this + offsets::velocity) = velocity;
	}

	qangle get_angles()
	{
		return *(qangle*)((dword)this + offsets::angles);
	}

	void set_angles(cvector angles)
	{
		*(cvector*)((dword)this + offsets::angles) = angles;
	}

	qangle get_punch()
	{
		return *(qangle*)((dword)this + offsets::aimpunch);
	}

	float get_simulation_time()
	{
		return *(float*)((dword)this + offsets::sim_time);
	}

	void set_simulation_time(float sim_time)
	{
		*(float*)((dword)this + offsets::sim_time) = sim_time;
	}

	icollideable* get_collideable()
	{
		typedef icollideable*(__thiscall* get_collideable_fn)(void*);
		return vfunc< get_collideable_fn>(this, 3)(this);
	}

	iclientnetworkable* get_clientnetworkable() { return (iclientnetworkable*)((dword)this + 0x8); }

	iclientrenderable* get_clientrenderable() { return (iclientrenderable*)((dword)this + 0x4); }

	/*const matrix3x4_t& get_coord_frame()
	{
		//const auto collideable = this->get_collideable();
		//if (!collideable) return nullptr;

		return this->get_collideable()->collision_to_world_transform();
		//return *(matrix3x4_t*)((dword)this + offsets::coord_frame);
	}*/

	//максимум 128 костей
	//BONE_USED_BY_HITBOX 0x00000100
	bool get_hitbox_matrix(matrix3x4_t* bone_to_world_out, float current_time = global::curtime)
	{
		auto renderable = get_clientrenderable();
		if (!renderable) return false;
		return renderable->setup_bones(bone_to_world_out, 128, 0x100, current_time);
	}

	cvector get_hitbox(int hitbox_id, matrix3x4_t* matrix = nullptr, float curtime = global::curtime)
	{
		if (!matrix && !get_hitbox_matrix(matrix, curtime)) return cvector();

		return cvector(matrix[hitbox_id][0][3], matrix[hitbox_id][1][3], matrix[hitbox_id][2][3]);
	}

	//bool visible(cvector to)
	//{
	//	return visibility_check(global::local_observed, this, to);
		/*if (to.IsZero()) return false;

		cvector from = global::local_observed->get_eye_pos();
		if (from.IsZero()) return false;

		itracefilter filter;
		filter.skip = global::local_observed;

		ray_t ray; ray.Init(from, to);

		trace_t trace;

		_engine_trace->trace_ray(ray, MASK_SOLID /*MASK_ALL, &filter, &trace);

		return (trace.m_pEnt == this && !trace.allsolid);*/
	//}

	//bool hitbox_visible(int id = 0, matrix3x4_t* matrix = nullptr)
	//{
	//	return this->visible(this->get_hitbox(id, matrix, global::curtime));
	//}

	cweapon* get_weapon()
	{
		dword weapon_handle = *(dword*)((dword)this + offsets::active_weapon);
		if (weapon_handle == 0x0) return nullptr;
		cweapon* weapon = (cweapon*)_ent_list->get_centity_from_handle(weapon_handle);
		return weapon;
	}

	//AUTOWALL: фикс ошибок
	/*void get_bullettype_parameters(
		int iBulletType,
		float& fPenetrationPower,
		float& flPenetrationDistance)
	{
		//MIKETODO: make ammo types come from a script file.
		if (IsAmmoType(iBulletType, BULLET_PLAYER_50AE))
		{
			fPenetrationPower = 30;
			flPenetrationDistance = 1000.0;
		}
		else if (IsAmmoType(iBulletType, BULLET_PLAYER_762MM))
		{
			fPenetrationPower = 39;
			flPenetrationDistance = 5000.0;
		}
		else if (IsAmmoType(iBulletType, BULLET_PLAYER_556MM) ||
			IsAmmoType(iBulletType, BULLET_PLAYER_556MM_BOX))
		{
			fPenetrationPower = 35;
			flPenetrationDistance = 4000.0;
		}
		else if (IsAmmoType(iBulletType, BULLET_PLAYER_338MAG))
		{
			fPenetrationPower = 45;
			flPenetrationDistance = 8000.0;
		}
		else if (IsAmmoType(iBulletType, BULLET_PLAYER_9MM))
		{
			fPenetrationPower = 21;
			flPenetrationDistance = 800.0;
		}
		else if (IsAmmoType(iBulletType, BULLET_PLAYER_BUCKSHOT))
		{
			fPenetrationPower = 0;
			flPenetrationDistance = 0.0;
		}
		else if (IsAmmoType(iBulletType, BULLET_PLAYER_45ACP))
		{
			fPenetrationPower = 15;
			flPenetrationDistance = 500.0;
		}
		else if (IsAmmoType(iBulletType, BULLET_PLAYER_357SIG))
		{
			fPenetrationPower = 25;
			flPenetrationDistance = 800.0;
		}
		else if (IsAmmoType(iBulletType, BULLET_PLAYER_57MM))
		{
			fPenetrationPower = 30;
			flPenetrationDistance = 2000.0;
		}
		else
		{
			// What kind of ammo is this?
			Assert(false);
			fPenetrationPower = 0;
			flPenetrationDistance = 0.0;
		}
	}*/

	centity* get_spec_player()
	{
		auto spec_handler = *(dword*)(this + offsets::obs_target);
		if (spec_handler == 0x0) return nullptr;
		return _ent_list->get_centity_from_handle(spec_handler);
	}

	const model_t* get_model()
	{
		const auto renderable = this->get_clientrenderable();
		if (!renderable) return nullptr;

		return renderable->get_model();
	}

	int get_spec_mode()
	{
		return *(int*)((dword)this + offsets::obs_mode);
	}

	int get_move_type()
	{
		return *(int*)((dword)this + offsets::move_type);
	}
};

class cweapon
{
public:
	fileweaponinfo_t& get_data()
	{
		return game::signatures::get_wpn_data();
	}

	float next_primary_attack()
	{
		if (!this || this->get_weaponid() == weapon_none) return (float)INT_MAX;

		return *(float*)((dword)this + offsets::next_prim_attack);
	}

	float next_secondary_attack()
	{
		if (!this || this->get_weaponid() == weapon_none) return (float)INT_MAX;

		return *(float*)((dword)this + offsets::next_sec_attack);
	}

	// TODO: переработать все эти is_%weapon_name% в один get_type, а лучше найти vfunc
	//
	//хз что с названием, но каждый раз в начале непон€тные символы (2-6 штук)
	// q??weapon_%name%
	//bool is_pistol(/*fileweaponinfo_t& p_data*/)
	/*{
		if (!this) return false;

		//auto name = global::wpn_data.szClassName;
		auto id = this->get_weaponid();

		//if (string(name).empty()) return false;

		return (id == weapon_glock || id == weapon_usp || id == weapon_deagle ||
				id == weapon_elites || id == weapon_fiveseven || id == weapon_p228);	

		//return (global::wpn_data.m_WeaponType == weapontype_pistol);
	}*/

	/*bool is_misc()
	{
		if (!this) return false;

		auto id = this->get_weaponid();

		return (id == weapon_c4 || id == weapon_smoke_grenade || id == weapon_flashbang || id == weapon_he_grenade || id == weapon_knife || id == weapon_none);
	}*/

	/*bool is_sniper()
	{
		if (!this) return false;

		auto id = this->get_weaponid();

		return (id == weapon_scout || id == weapon_awp || id == weapon_g3sg1 || id == weapon_sg550);
	}*/
	//
	//

	weap_type get_type()
	{
		if (!this || this->get_weaponid() == weapon_none) return weap_max;

		const auto id = this->get_weaponid();

		switch (id)
		{
		case weapon_glock: case weapon_usp: case weapon_deagle:
		case weapon_elites: case weapon_fiveseven: case weapon_p228:
			return weap_pistol;
		case weapon_scout: case weapon_awp: case weapon_g3sg1: case weapon_sg550:
			return weap_snip;
		case weapon_famas: case weapon_galil: case weapon_ak47: case weapon_m4a1: 
		case weapon_aug: case weapon_sg552: 
			return weap_rifle;
		case weapon_mac10: case weapon_tmp: case weapon_mp5: case weapon_ump: case weapon_p90:
			return weap_sub;
		case weapon_m3: case weapon_xm014: 
			return weap_shot;
		case weapon_m249:
			return weap_heavy;
		default: return weap_misc;
		}

		return weap_max;
	}

	int get_clip1()
	{
		if (!this || this->get_weaponid() == weapon_none) return -1;

		return *(int*)((dword)this + offsets::clip1);
	}

	// поскольку wpndata ебнута€ будет так
	int get_maxclip1()
	{
		if (!this || this->get_weaponid() == weapon_none || this->get_type() == weap_misc) return -1;

		switch (this->get_weaponid())
		{
		case weapon_ak47: case weapon_m4a1: case weapon_elites:
		case weapon_aug: case weapon_sg550: case weapon_tmp:
		case weapon_mac10: case weapon_mp5:
			return 30;
		case weapon_ump: case weapon_famas: return 25;
		case weapon_galil: return 35;
		case weapon_m249: return 100;
		case weapon_g3sg1: case weapon_glock: case weapon_fiveseven: return 20;
		case weapon_awp: return 10;
		case weapon_usp: return 12;
		case weapon_xm014: case weapon_deagle: return 7;
		case weapon_m3: return 8;
		case weapon_p228: return 13;
		case weapon_p90: return 50;
		}

		return -1;
	}

	/*
	// start 195
	weaponid 365
	inaccuracy 376
	spread 377
	accuracypenalty 378
	*/
	int get_weaponid()
	{
		if (!this) return weapon_none;

		typedef int(__thiscall* get_weaponid_fn)(void*);
		//GetWeaponID_t GetWeaponIDFunction = (GetWeaponID_t)(this + 365);
		return vfunc< get_weaponid_fn >(this, 365)(this);

		//return ((GetWeaponID_t)(*(PDWORD)(*(PDWORD)(this) + WEAPONIDOFFSET)))(this);		
	}

	void update_acc_penalty()
	{
		if (!this) return;
		typedef void(__thiscall* update_acc_penalty_fn)(void*);
		vfunc< update_acc_penalty_fn >(this, 378)(this);
	}

	float get_spread()
	{
		if (!this) return 0.f;
		typedef float(__thiscall* update_acc_penalty_fn)(void*);
		return vfunc< update_acc_penalty_fn >(this, 377)(this);
	}

	float get_inacc()
	{
		if (!this) return 0.f;
		typedef float(__thiscall* update_acc_penalty_fn)(void*);
		return vfunc< update_acc_penalty_fn >(this, 376)(this);
	}

	string get_name()
	{
		if (!this) return "none";

		const auto id = this->get_weaponid();
		if (id <= 0 || id >= 30) return "none";

		return weapon_names[id];
	}
};

class cusercmd
{
public:
	/*cusercmd& operator =(const cusercmd& src)
	{
		if (this == &src)
			return *this;

		command_number = src.command_number;
		tick_count = src.tick_count;
		viewangles = src.viewangles;
		forwardmove = src.forwardmove;
		sidemove = src.sidemove;
		upmove = src.upmove;
		buttons = src.buttons;
		impulse = src.impulse;
		weaponselect = src.weaponselect;
		weaponsubtype = src.weaponsubtype;
		random_seed = src.random_seed;
		mousedx = src.mousedx;
		mousedy = src.mousedy;

		hasbeenpredicted = src.hasbeenpredicted;

		return *this;
	}

	cusercmd(const cusercmd& src)
	{
		*this = src;
	}*/

	CRC32_t GetChecksum(void) const
	{
		CRC32_t crc;

		CRC32_Init(&crc);
		CRC32_ProcessBuffer(&crc, &command_number, sizeof(command_number));
		CRC32_ProcessBuffer(&crc, &tick_count, sizeof(tick_count));
		CRC32_ProcessBuffer(&crc, &viewangles, sizeof(viewangles));
		CRC32_ProcessBuffer(&crc, &forwardmove, sizeof(forwardmove));
		CRC32_ProcessBuffer(&crc, &sidemove, sizeof(sidemove));
		CRC32_ProcessBuffer(&crc, &upmove, sizeof(upmove));
		CRC32_ProcessBuffer(&crc, &buttons, sizeof(buttons));
		CRC32_ProcessBuffer(&crc, &impulse, sizeof(impulse));
		CRC32_ProcessBuffer(&crc, &weaponselect, sizeof(weaponselect));
		CRC32_ProcessBuffer(&crc, &weaponsubtype, sizeof(weaponsubtype));
		CRC32_ProcessBuffer(&crc, &random_seed, sizeof(random_seed));
		CRC32_ProcessBuffer(&crc, &mousedx, sizeof(mousedx));
		CRC32_ProcessBuffer(&crc, &mousedy, sizeof(mousedy));
		CRC32_Final(&crc);

		return crc;
	}

	BYTE u1[4];

	// For matching server and client commands for debugging
	int		command_number;

	// the tick the client created this command
	int		tick_count;

	// Player instantaneous view angles.
	qangle	viewangles;
	// Intended velocities
	//	forward velocity.
	float	forwardmove;
	//  sideways velocity.
	float	sidemove;
	//  upward velocity.
	float	upmove;
	// Attack button states
	int		buttons;
	// Impulse command issued.
	byte    impulse;
	// Current weapon id
	int		weaponselect;
	int		weaponsubtype;

	int		random_seed;	// For shared random functions

	short	mousedx;		// mouse accum in x from create move
	short	mousedy;		// mouse accum in y from create move

	// Client only, tracks whether we've predicted this command at least once
	bool	hasbeenpredicted;
};

class c_verified_usercmd
{
public:
	cusercmd			m_cmd;
	unsigned long		m_crc;
};

/////////////////////////////////////////////////////////
//		   very big thanks to catalindragan22
//https://www.unknowncheats.me/forum/2648983-post198.html
class key_value
{
public:
	int m_iKeyName;
	char* m_sValue;
	wchar_t* m_wsValue;

	union
	{
		int m_iValue;
		float m_flValue;
		void* m_pValue;
		unsigned char m_Color[4];
	};

	char	m_iDataType;
	char	m_bHasEscapeSequences;
	char	m_bEvaluateConditionals;
	char	unused[1];

	key_value* m_pPeer;
	key_value* m_pSub;
	key_value* m_pChain;
};

class ifile_system;

class key_values
{
public:
	static key_values* singleton()
	{
		static key_values* instance = new key_values;
		return instance;
	}

	bool load_from_buffer(key_value* key_valuez, char const* resource_name, const char* buffer, ifile_system* file_system = nullptr, const char* path_id = NULL)
	{
		using originalfn = int(__thiscall*)(key_value*, char const*, const char*, ifile_system*, const char*);
		uint64_t signature = memory::pattern("engine.dll", "55 8B EC 83 EC 38 53 8B 5D 0C");

		static originalfn load_from_the_buffer = (originalfn)signature;
		return load_from_the_buffer(key_valuez, resource_name, buffer, file_system, path_id);
	}

	key_value* initialize(key_value* key_valuez, char* name)
	{
		using originalfn = key_value * (__thiscall*)(key_value*, char*);

		uint64_t signature = memory::pattern("engine.dll", "FF 15 ? ? ? ? 83 C4 08 89 06 8B C6");
		signature -= 0x42; static originalfn initialize = (originalfn)signature;

		return initialize(key_valuez, name);
	}
};
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////