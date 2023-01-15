#pragma once
#include "includes.h"
#include "definitions.h"
#include "game def's.h"
#include "color.h"

class cusercmd;
class centity;
class cweapon;

namespace global
{
	hwnd window;
	hmodule dll;
	bool key[0xFE + 1]; // все кнопки (вкл/выкл удержание)
	bool key_do[0xFE + 1]; // все кнопки (вкл/выкл нажатие)
	bool key_click[0xFE + 1]; // все кнопки (на клик)
	float key_timer[0xFE + 1]; // все кнопки (сколько по времени она была нажата)
	point mouse; // позиция курсора
	rect screen; // размер экрана
	cusercmd* cmd;
	bool map_changed;
	bool* lock_cursor;
	bool sendpacket;
	int chocked_packets;
	int local_id;
	centity* local;
	cweapon* weapon;
	centity* local_observed;
	//fileweaponinfo_t wpn_data = fileweaponinfo_t();
	float curtime;
	float realtime;
	bool unhook = false;
	//matrix4x4 w2s_matrix;
}

namespace server
{
	int max_players = 0;
	//int max_entity = 0;

	struct
	{
		int type = 0;
	} local;

	struct
	{
		//int health = 0;
		//int type = 0;
		//int clip = 0;
		//int team = 0;
		//int lifestate = 0;
		//string weapon = "";
		//string name = "";
		//centity* spec_player = nullptr;
		//float drawable_timer = 0.f;
		//bool drawable = false;
		//bool valid = false;
	} players[64];//, local;

	/*struct
	{
		matrix3x4_t coord_frame;
		bool valid = false;
	} entity[512];*/
}