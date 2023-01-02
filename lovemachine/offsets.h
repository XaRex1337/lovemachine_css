#pragma once
#include "game shit.h"
#include "dt_recv.h"

//ofstream myfile;

namespace offsets
{
	vector<RecvTable*> tables;

	dword health;
	dword flags;
	dword life_state;
	dword team;
	dword active_weapon;
	dword clip1;
	dword next_prim_attack;
	dword next_sec_attack;
	dword tickbase;
	dword origin;
	dword view_offset;
	dword velocity;
	dword angles;
	dword sim_time;
	dword account; // money
	dword aimpunch;
	dword shots_fired;
	dword obs_target;
	dword obs_mode;
	dword armor;
	dword c4_blow;
	dword c4_timer;
	//dword coord_frame;
	dword move_type = 0x178;
	dword have_defuser;
	//dword c4_defuse;

	// TODO : переработать cout в console::write
	const char* initialize_tables()
	{
		tables.clear(); // очистка массива

		auto p_client = game::interfaces::client;

		if (!p_client)
			return "/fail/ failed to get client";
		
		auto p_class = p_client->get_all_classes(); // все клиентские классы
		if (!p_class)
			return "/fail/ failed to get classes";

		while (p_class)
		{
			auto table = p_class->recv_table; // таблица класса
			tables.push_back(table); // добавляем в список таблицу

			p_class = p_class->next; // переходим к следующему классу
		}

		return "/tables/ succesfull";
	}

	//TODO : понять как работает эта рекурсивная функция
	dword get_property(RecvTable* table, const char* property_name, RecvProp** property = NULL)
	{
		int extraOffset = 0;
		for (int i = 0; i < table->m_nProps; ++i)
		{
			RecvProp* recvProp = &table->m_pProps[i];
			RecvTable* child = recvProp->m_pDataTable;

			if (child && (child->m_nProps > 0))
			{
				int tmp = get_property(child, property_name, property);

				if (tmp)
				{
					extraOffset += (recvProp->m_Offset + tmp);
				}
			}

			if (_stricmp(recvProp->m_pVarName, property_name))
				continue;

			if (property)
				*property = recvProp;

			return (recvProp->m_Offset + extraOffset);
		}

		return extraOffset;
	}

	dword get_offset(const char* table_name, const char* property_name)
	{
		if (tables.empty())
			return 0x0;

		RecvTable* p_table = NULL;

		for (auto table : tables)
		{
			if (!table)
				continue;

			if (_stricmp(table->m_pNetTableName, table_name) == 0)
			{
				p_table = table;
				break;
			}
		}

		dword offset = get_property(p_table, property_name);

		if (offset) console::write_hex(string("/offset/ ") + table_name + string(" | ") + property_name, offset, green);
		else console::write(string("/offset/ ") + table_name + string(" | ") + property_name + string(" wasn't found"), red);

		return offset;
	}

	// TODO : доработать
	void dump()
	{
		for (auto table : tables)
		{
			if (!table) continue;

			for (int i = 0; i < table->m_nProps; ++i)
			{
				RecvProp* recvProp = &table->m_pProps[i];

				//myfile << table->m_pNetTableName << " | " << recvProp->m_pVarName << endl;

				RecvTable* child = recvProp->m_pDataTable;
				if (child && (child->m_nProps > 0))
				{
					for (int j = 0; j < child->m_nProps; j++)
					{
						//myfile << table->m_pNetTableName << " | " << child->m_pNetTableName << " | " << child->m_pProps[j].m_pVarName << endl;
					}
				}
			}
		}
	}

	void find_them()
	{
		auto result = initialize_tables();
		cout << result << endl;

		//dump();

		health = get_offset("DT_CSPlayer", "m_iHealth");
		flags = get_offset("DT_CSPlayer", "m_fFlags");
		life_state = get_offset("DT_BasePlayer", "m_LifeState");
		team = get_offset("DT_CSPlayer", "m_iTeamNum");
		active_weapon = get_offset("DT_CSPlayer", "m_hActiveWeapon");
		clip1 = get_offset("DT_BaseCombatWeapon", "m_iClip1");
		next_prim_attack = get_offset("DT_BaseCombatWeapon", "m_flNextPrimaryAttack");
		next_sec_attack = get_offset("DT_BaseCombatWeapon", "m_flNextSecondaryAttack");
		tickbase = get_offset("DT_CSPlayer", "m_nTickBase");
		origin = get_offset("DT_BasePlayer", "m_vecOrigin");
		view_offset = get_offset("DT_CSPlayer", "m_vecViewOffset[0]");
		velocity = get_offset("DT_CSPlayer", "m_vecVelocity[0]");
		angles = get_offset("DT_CSPlayer", "m_angEyeAngles[0]"); // m_angRotation ?
		sim_time = get_offset("DT_CSPlayer", "m_flSimulationTime");
		account = get_offset("DT_CSPlayer", "m_iAccount");
		aimpunch = get_offset("DT_CSPlayer", "m_vecPunchAngle");
		shots_fired = get_offset("DT_CSPlayer", "m_iShotsFired");
		obs_target = get_offset("DT_CSPlayer", "m_hObserverTarget");
		obs_mode = get_offset("DT_CSPlayer", "m_iObserverMode");
		armor = get_offset("DT_CSPlayer", "m_ArmorValue"); // TODO: рядом есть m_iFrags и m_iDeaths, добавить в info
		c4_blow = get_offset("DT_PlantedC4", "m_flC4Blow");
		c4_timer = get_offset("DT_PlantedC4", "m_flTimerLength");
		//coord_frame = get_offset("DT_CSPlayer", "m_fFlags") - sizeof(Vector) - sizeof(Vector) - sizeof(matrix3x4_t);
		have_defuser = angles + sizeof(qangle);//get_offset("CCSPlayer", "m_bHasDefuser");
		//c4_defuse = get_offset("DT_PlantedC4", "m_flDefuseCountDown");
	}
}