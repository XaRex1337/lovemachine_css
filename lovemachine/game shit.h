#pragma once
#include "includes.h"
#include "definitions.h"
#include "memory.h"
#include "console.h"
#include "interfaces.h"

namespace game
{
	namespace interfaces
	{
		ivpanel* panel;
		isurface* surface;
		icliententitylist* ent_list;
		ivengineclient* engine; // client
		ibaseclientdll* client; // base client dll
		ivdebugoverlay* debug_overlay;
		imaterialsystem* mat_sys;
		ivmodelrender* model_render;
		ivmodelinfo* model_info; // client
		ivrenderview* render_view;
		iplayerinfomanager* pl_info_manager;
		ienginetrace* engine_trace;
		//ienginesound* engine_sound;
		igameeventmanager* event_manager;
		icvar* cvar;
		cglobalvars* globals;
		cinput* input;
		iphysicssurfaceprops* physics;

		void find_them()
		{
			panel = memory::pinterface<ivpanel>("vgui2.dll", "VGUI_Panel009");
			console::write_hex("/interface/ panel", (dword)panel, darkgreen);

			surface = memory::pinterface<isurface>("vguimatsurface.dll", "VGUI_Surface030");
			console::write_hex("/interface/ surface", (dword)surface, darkgreen);

			ent_list = memory::pinterface<icliententitylist>("client.dll", "VClientEntityList003");
			console::write_hex("/interface/ ent_list", (dword)ent_list, darkgreen);

			engine = memory::pinterface<ivengineclient>("engine.dll", "VEngineClient014");
			console::write_hex("/interface/ engine", (dword)engine, darkgreen);

			client = memory::pinterface<ibaseclientdll>("client.dll", "VClient017");
			console::write_hex("/interface/ client", (dword)client, darkgreen);

			if (client) console::write_hex("/function/ client->get_all_classes()", (dword)client->get_all_classes(), darkgreen);

			debug_overlay = memory::pinterface<ivdebugoverlay>("engine.dll", "VDebugOverlay003");
			console::write_hex("/interface/ debug_overlay", (dword)debug_overlay, darkgreen);

			mat_sys = memory::pinterface<imaterialsystem>("materialsystem.dll", "VMaterialSystem080");
			console::write_hex("/interface/ mat_sys", (dword)mat_sys, darkgreen);

			model_render = memory::pinterface<ivmodelrender>("engine.dll", "VEngineModel016");
			console::write_hex("/interface/ model_render", (dword)model_render, darkgreen);

			model_info = memory::pinterface<ivmodelinfo>("engine.dll", "VModelInfoClient006");
			console::write_hex("/interface/ model_info", (dword)model_info, darkgreen);

			render_view = memory::pinterface<ivrenderview>("engine.dll", "VEngineRenderView014");
			console::write_hex("/interface/ render_view", (dword)render_view, darkgreen);

			pl_info_manager = memory::pinterface<iplayerinfomanager>("server.dll", "PlayerInfoManager002");
			console::write_hex("/interface/ player_info_manager", (dword)pl_info_manager, darkgreen);

			engine_trace = memory::pinterface<ienginetrace>("engine.dll", "EngineTraceClient003");
			console::write_hex("/interface/ engine_trace", (dword)engine_trace, darkgreen);

			cvar = memory::pinterface<icvar>("vstdlib.dll", "VEngineCvar004");
			console::write_hex("/interface/ cvar", (dword)cvar, darkgreen);

			//engine_sound = memory::pinterface<ienginesound>("engine.dll", "IEngineSoundClient003");
			//console::write_hex("/interface/ engine_sound", (dword)engine_sound, darkgreen);

			event_manager = memory::pinterface<igameeventmanager>("engine.dll", "GAMEEVENTSMANAGER002");
			console::write_hex("/interface/ event_manager", (dword)event_manager, darkgreen);

			physics = memory::pinterface<iphysicssurfaceprops>("vphysics.dll", "VPhysicsSurfaceProps001");
			console::write_hex("/interface/ physics", (dword)physics, darkgreen);
		}
	}

	namespace signatures
	{
		dword d3d9_device;
		//iclientmode* clientmode;
		dword clientmode;
		dword bullet_params;
		get_data_fn get_wpn_data;
		//get_bullet_type_fn get_bullet_type;
		cclientstate* clientstate;

		typedef void (*ClipTraceToPlayers_t)(const Vector&, const Vector&, unsigned int, itracefilter*, trace_t*);
		ClipTraceToPlayers_t  ClipTraceToPlayers;

		void find_them()
		{
			d3d9_device = **reinterpret_cast<dword**>(memory::pattern("shaderapidx9.dll", "A1 ? ? ? ? 8D 53 08") + 0x1);
			console::write_hex("/signature/ d3d9_device", d3d9_device, darkgreen);

			clientmode = **reinterpret_cast<dword**>(memory::pattern("client.dll", "8B 0D ? ? ? ? 8B 01 5D FF 60 28 CC") + 0x2);
			console::write_hex("/signature/ clientmode", clientmode, darkgreen);

			// (c) mr-nv <3
			bullet_params = memory::pattern("client.dll", "55 8B EC 56 8B 75 08 68 ? ? ? ? 56 E8 ? ? ? ? 83 C4 08 84 C0");
			console::write_hex("/signature/ bullet_params", bullet_params, darkgreen);

			// TODO: ÎÁÍÎÂÈÒÜ È ÂÅÐÍÓÒÜ
			// thx catalindragan22 (c) // uc
			//clientstate = *reinterpret_cast<cclientstate**>(memory::pattern("engine.dll", "B9 ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? 83 78 14 00") + 0x1);
			console::write_hex("/signature/ clientstate", (dword)clientstate, darkgreen);
			//console::write_hex("netchannel", (dword)clientstate->m_NetChannel, darkgreen);

			dword weapon_data = memory::pattern("client.dll", "0F B7 81 ? ? ? ? 50 E8 ? ? ? ? 83 C4 04 C3");
			console::write_hex("/signature/ weapon_data", weapon_data, darkgreen);
			get_wpn_data = (get_data_fn)(weapon_data);
			console::write_hex("/signature/ get_wpn_data", (dword)get_wpn_data, darkgreen);

			dword lock_cursor = memory::pattern("vguimatsurface.dll", "A3 ? ? ? ? C6 05") + 0x7;
			global::lock_cursor = *(bool**)(lock_cursor);
			console::write_hex("/signature/ lock_cursor", (dword)global::lock_cursor, darkgreen);

			dword cliptracetoplayers = memory::pattern("client.dll", "53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 81 EC ? ? ? ? 8B 43 18");
			ClipTraceToPlayers = (ClipTraceToPlayers_t)(cliptracetoplayers);
			console::write_hex("/signature/ ClipTraceToPlayers", (dword)ClipTraceToPlayers, darkgreen);

			/*dword bullet_type = memory::pattern("server.dll", "E8 ? ? ? ? F3 0F 10 43 ? F3 0F 10 A5 ? ? ? ?") + 0x7;
			get_bullet_type = (get_bullet_type_fn)(bullet_type);
			console::write_hex("/signature/ lock_cursor", (dword)get_bullet_type, darkgreen);*/
		}
	}

	void find()
	{
		interfaces::find_them();
		signatures::find_them();
	}
}

#define _panel game::interfaces::panel
#define _surface game::interfaces::surface
#define _ent_list game::interfaces::ent_list
#define _engine game::interfaces::engine
#define _client game::interfaces::client
#define _debug_overlay game::interfaces::debug_overlay
#define _mat_sys game::interfaces::mat_sys
#define _model_render game::interfaces::model_render
#define _model_info game::interfaces::model_info
#define _render_view game::interfaces::render_view
#define _engine_trace game::interfaces::engine_trace
#define _engine_sound game::interfaces::engine_sound
#define _event_manager game::interfaces::event_manager
#define _cvar game::interfaces::cvar
#define _globals game::interfaces::globals
#define _input game::interfaces::input
#define _clientmode game::signatures::clientmode
#define _phys game::interfaces::physics
//#define _clientstate game::signatures::clientstate