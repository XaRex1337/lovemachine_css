#pragma once
#include "includes.h"
#include "definitions.h"
#include "console.h"
#include "memory.h"
#include "global.h"
#include "interfaces.h"
#include "game def's.h"
#include "game shit.h"
#include "game classes.h"
#include "d3d.h"
#include "menu.h"
#include "legit.h"
#include "models.h"
#include "esp.h"
#include "misc.h"
#include "events.h"
#include "surface.h"
#include "net shit.h"

using namespace d3d;
using namespace game;

namespace hooks
{	
	wndproc o_wndproc;
	LRESULT __stdcall wndproc_hook(hwnd wnd, uint msg, wparam w_param, lparam l_param)
	{
		global::realtime = (float)GetTickCount64() / 1000.f;

		switch (msg)
		{
		case WM_LBUTTONDOWN:
			global::key[VK_LBUTTON] = true;
			if (global::key_timer[VK_LBUTTON] == 0.f)
				global::key_timer[VK_LBUTTON] = global::realtime;
			//if (legit::aimbot::smac_delay) return true;
			break;
		case WM_LBUTTONUP:
			global::key[VK_LBUTTON] = false;
			global::key_timer[VK_LBUTTON] = 0.f;
			break;
		case WM_RBUTTONDOWN:
			global::key[VK_RBUTTON] = true;
			break;
		case WM_RBUTTONUP:
			global::key[VK_RBUTTON] = false;
			break;
		case WM_MBUTTONDOWN:
			global::key[VK_MBUTTON] = true;
			break;
		case WM_MBUTTONUP:
			global::key[VK_MBUTTON] = false;
			break;
		case WM_XBUTTONDOWN:
			global::key[VK_XBUTTON1 + (GET_XBUTTON_WPARAM(w_param) - 1)] = true; // GET_XBUTTON_WPARAM = кака€ по значению кнопка(бокова€)
			break;
		case WM_XBUTTONUP:
			global::key[VK_XBUTTON1 + (GET_XBUTTON_WPARAM(w_param) - 1)] = false; // GET_XBUTTON_WPARAM = кака€ по значению кнопка(бокова€)
			break;
		case WM_KEYDOWN:
			global::key[w_param] = true;
			if (w_param == VK_F5 || w_param == VK_SNAPSHOT)
				return CallWindowProc(o_wndproc, wnd, msg, w_param, l_param);
			break;
		case WM_KEYUP:
			global::key[w_param] = false;
			if (w_param == VK_F5 || w_param == VK_SNAPSHOT)
				return CallWindowProc(o_wndproc, wnd, msg, w_param, l_param);
			break;
		case WM_MOUSEMOVE:
			global::mouse.x = (signed short)(l_param);
			global::mouse.y = (signed short)(l_param >> 16);
			break;
		default: break;
		}

		if (sets->menu.opened && !sets->menu.panic)
			return true;

		/*if (_engine)
		{
			static bool do_it = false;

			if (_engine->is_connected() && _engine->in_game() && sets->misc.autojump && global::key[VK_SPACE])
			{
				_engine->clientcmd_unrestricted(GetTickCount64() % 2 ? "+jump" : "-jump");
				do_it = true;
			}
			else
			{
				if (do_it)
				{
					_engine->clientcmd_unrestricted("-jump");
					do_it = false;
				}
			}
		}*/
		
		return CallWindowProc(o_wndproc, wnd, msg, w_param, l_param);
	}

	bool key[0xFE + 1];
	memory::vthook* d3d9;
	using endscene_fn = long(__stdcall*)(IDirect3DDevice9* device);
	endscene_fn o_endscene;
	long __stdcall endscene_hook(IDirect3DDevice9* device)
	{
		static bool once = true, fonts = false;

		if (once)
		{
			d3d::device = device;
			console::write_hex("/d3d/ device", (dword)device, darkgreen);
			font::setup(font::tab, "Gotham Pro Medium", 24, FW_DONTCARE); // smallest_pixel-7 24 bold
			font::setup(font::cont, "Gotham Pro", 15, fw_medium); // smallest_pixel-7 14 bold // 15, FW_DONTCARE
			font::setup(font::hitmarker_big, "Gotham Pro Black", 30, FW_BOLD);
			font::setup(font::hitmarker_small, "Gotham Pro Medium", 19, FW_BOLD);
			font::setup(font::esp, "Gotham Pro Medium", 16, FW_DONTCARE);
			//prim::generate_texture(&prim::primitive, D3DCOLOR_ARGB(255, 255, 255, 255));
			font::reset();
			global::screen = get_screen_size();
			//ZeroMemory(global::w2s_matrix, sizeof(matrix4x4));
			once = false;
		}

		d3d9->rehook(); // зачем?

		if (sets->menu.panic)
		{
			sets->menu.opened = false;

			if (global::key[VK_NUMPAD5])
				sets->menu.panic = false;

			return o_endscene(device);
		}

		if (!_engine->in_game() && !global::map_changed)
		{
			events::bomb_timer::defuse_time = events::bomb_timer::explosion_time = events::hitmarker::timer = events::hitmarker::kill_timer = 0.f;
			//ZeroMemory(legit::backtrack::records, sizeof(legit::backtrack::records));
			legit::backtrack::draw = false;
			for (int id = 0; id < 64; id++)
				for (int tick = 0; tick < 10; tick++)
					legit::backtrack::records[id][tick].valid = false;
			legit::backtrack::best_record.valid = false;
			if (!fonts)
			{
				surf::font::setup(surf::font::esp, "Gotham Pro", 17, fw_normal, ff_antialias | ff_dropshadow);
				fonts = true;
			}
			global::map_changed = true;
		}

		//TODO : усовершенствовать
		for (int key_id = 0; key_id < 0xFE + 1; key_id++)
		{
			if (key[key_id] != global::key[key_id])
			{
				key[key_id] = global::key[key_id];

				global::key_click[key_id] = global::key[key_id];
			}
			else
			{
				global::key_click[key_id] = false;
			}

			if (global::key_click[key_id])
			{
				global::key_do[key_id] = !global::key_do[key_id];
			}
		}

		/*if (_engine->is_screenshoting())
		{
			console::write("TAKING SCREENSHOT", red);
			return o_endscene(device);
		}*/

		/*if (_engine && _engine->is_connected() && _engine->in_game() && global::cmd)
		{
			auto point = cvector(10, 10, 10);
			cvector screen;
			auto result = w2s_keybode(point, screen);//_debug_overlay->screen_position(point, screen);
			//cout << "w2s result : " << result << ", screen.x : " << screen.x << ", screen.y : " << screen.y << endl;
			if (result == 1 && (obj::in_range(0, 0, screen.x, screen.y, global::screen.right, global::screen.bottom)))//w2s(point, screen))
			{
				prim::filled_box(screen.x - 5, screen.y - 5, screen.x + 5, screen.y + 5, color(255, 0, 0));
			}
		}*/

		//if (_engine->is_connected() && _engine->in_game() && global::cmd && global::cmd->command_number != 0 && global::local)
		//{
			/*cvector angle = global::cmd->viewangles;
			static auto stangle = angle;

			if (stangle != angle)
			{
				auto pmatrix = _engine->w2s_matrix();
				for (int i = 0; i < 4; i++)
					for (int j = 0; j < 4; j++)
					{
						global::w2s_matrix[i][j] = pmatrix[i][j];
						cout << "matrix[" << i << "][" << j << "] -> " << global::w2s_matrix[i][j] << ((i == 0 && j == 3) ? "\n" : ", ");
					}

				stangle = angle;
			}*/

		static bool st_crosshair = true;

		if (global::local && global::local->valid() && (sets->visuals.crosshair == 1 || (sets->visuals.crosshair == 2 && server::local.type == weap_snip)))//&& ((sets->visuals.crosshair == 1 && global::weapon->get_weaponid() != weapon_none) || (sets->visuals.crosshair == 2 && global::weapon->is_sniper())))
		{
			if (st_crosshair)
			{
				_engine->clientcmd_unrestricted("crosshair 0");
				st_crosshair = false;
			}

			int centerx = global::screen.right / 2;
			int centery = global::screen.bottom / 2;

			prim::filled_box(centerx - 5, centery, centerx + 6, centery + 1, color::lm());
			prim::filled_box(centerx, centery - 5, centerx + 1, centery + 6, color::lm());
		}
		else
		{
			if (!st_crosshair)
			{
				_engine->clientcmd_unrestricted("crosshair 1");
				st_crosshair = true;
			}
		}

		//if (sets->visuals.enabled)
		//	esp::run();
	//}

	//if (!_engine->in_game()) global::map_changed = true;

	/*if (sets->legit.enabled && (sets->legit.backtrack.enabled || (sets->legit.aim.fov > 0.f && sets->legit.aim.kill_delay > 0.f)))
	{
		static bool reset = true;

		if (reset && (!_engine->is_connected() || !_engine->in_game() || !global::local || !global::local->valid()))
		{
			ZeroMemory(legit::backtrack::records, sizeof(legit::backtrack::records));
			legit::backtrack::best_record = legit::backtrack::crecord();
			legit::backtrack::best_fov = 9999.f;
			legit::aimbot::kill_delay = 0.f;

			reset = false;
		}
		else if (!reset && _engine->is_connected() && _engine->in_game() && global::local && global::local->valid())
		{
			reset = true;
		}
	}*/

		legit::trigger::draw();

		if (sets->visuals.enabled && _engine->in_game() && global::local)
		{
			//esp::draw();
			events::on_draw();
		}

		menu::draw();

		return o_endscene(device);
	}

	using reset_fn = long(__stdcall*)(IDirect3DDevice9 * device, D3DPRESENT_PARAMETERS * pp);
	reset_fn o_reset;
	long __stdcall reset_hook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pp)
	{
		console::write("d3d reset", darkred);

		font::restore();
		auto result = o_reset(device, pp);
		font::reset();
		surf::font::setup(surf::font::esp, "Gotham Pro", 17, fw_normal, ff_antialias | ff_dropshadow);
		global::screen = get_screen_size();
		//ZeroMemory(global::w2s_matrix, sizeof(matrix4x4));

		return result;
	}

	convar* sv_cheats = nullptr;

	memory::vthook* panel;
	using painttraverse_fn = void(__stdcall*)(unsigned int vguiPanel, bool forceRepaint, bool allowForce);
	painttraverse_fn o_painttraverse;
	void __stdcall painttraverse_hook(unsigned int vguiPanel, bool forceRepaint, bool allowForce)
	{
		o_painttraverse(vguiPanel, forceRepaint, allowForce);

		static unsigned int top_panel = 0;
		auto name = _panel->get_name(vguiPanel);
		//myfile << name << endl;
		if (top_panel == 0 && !strcmp(name, "MatSystemTopPanel"))
		{
			top_panel = vguiPanel;
			//surf::font::setup(surf::font::esp, "Gotham Pro Medium", 17, fw_normal, ff_antialias | ff_dropshadow);
			//surf::font::setup(surf::font::esp_big, "Gotham Pro Black", 30, fw_semibold, ff_antialias);
		}

		//if (!sv_cheats) sv_cheats = _cvar->find_var("sv_cheats");
		//else surf::font::draw(surf::font::esp_big, 100, 100, color::ptext(), null, to_str(sv_cheats->m_nValue).c_str());

		if (top_panel == 0 || top_panel != vguiPanel || !_engine->in_game() || !global::local || sets->menu.panic) return;

		//surf::font::draw(surf::font::esp, 100, 100, color(200, 200, 200), null, "text text text");

		if (sets->misc.draw_mode == 0) misc::draw::draw();

		if (!sets->visuals.enabled) return;

		esp::draw();

		/*const auto matrix = _engine->w2s_matrix();
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				global::w2s_matrix[i][j] = matrix[i][j];*/

		/*for (int id = 0; id < _engine->get_max_clients()/*_ent_list->get_highest_entity_index()*//*; id++)
		{
			auto entity = _ent_list->get_centity(id);
			if (!entity || !entity->valid() || entity->is_dormant()/* || entity->get_origin().IsZero()*//*) continue;

			//auto class_id = entity->get_clientnetworkable()->get_clientclass()->class_id;

			//if (class_id == CCSPlayer)
			//{
			//	server::max_players = id + 1;

				//if (entity->valid())
				//{
					auto weapon = entity->get_weapon();
					if (weapon)
					{
						auto type = weapon->get_type();
						if (type != weap_max)
						{
							if (id == global::local_id)
							{
								server::local.type = type;
							}
							else if (sets->visuals.enabled && sets->visuals.esp_filter[0] && sets->visuals.esp_show[3])
							{
								if (sets->visuals.esp_bar[3]) server::players[id].clip = weapon->get_clip1();
								server::players[id].weapon = weapon->get_name();
								server::players[id].valid = true;
							}
						}
						else
						{
							if (id == global::local_id) server::local.type = weap_max;
							else
							{
								server::players[id].clip = -1;
								server::players[id].weapon = "none";
							}
						}
					}
					//server::players[id].health = entity->get_clientnetworkable()->get_clientclass()->class_id;
				//}
			//}
			//else
			//{
			//	server::max_entity = id + 1;

			//	if (sets->visuals.enabled && sets->visuals.fancy_w2s)
			//	{
			//		server::entity[id].coord_frame = entity->get_coord_frame();
			//		server::entity[id].valid = true;
			//	}
			//}
		}*/

		/*for (int id = 0; id < min(256, _ent_list->get_highest_entity_index()); id++)
		{
			auto entity = _ent_list->get_centity(id);
			if (!entity || entity->get_origin().IsZero())
			{
				server::players[id].valid = false;
				server::entity[id].valid = false;
				continue;
			}

			//auto class_id = entity->get_clientnetworkable()->get_clientclass()->class_id;

			if (id < 32 && entity->valid())
			{
				auto weapon = entity->get_weapon();
				if (weapon)
				{
					auto type = weapon->get_type();
					if (type != weap_max)
					{
						if (id == global::local_id)
						{
							server::local.type = type;
						}
						else
						{
							server::players[id].clip = weapon->get_clip1();
							server::players[id].weapon = weapon->get_name();
							server::players[id].valid = true;
						}
					}
				}
			}
			else if (id > 32)
			{
				if (sets->visuals.enabled && sets->visuals.fancy_w2s)
				{
					server::entity[id].coord_frame = entity->get_coord_frame();
					server::entity[id].valid = true;
				}
			}
			else if (id < 32)
			{
				server::players[id].valid = false;
			}
		}*/

		// € пыталс€, но ничего не вышло
		/*static unsigned int correct_vgui = NULL;
		if (correct_vgui == NULL)
		{
			const char* vguipanel_name = game::interfaces::panel->get_name(vguiPanel);

			if (strstr(vguipanel_name, "Mat"))
			{
				console::write("/painttraverse/ correct vguipanel_name => " + string(vguipanel_name), green);
				correct_vgui = vguiPanel;
				pt_hooked = true;
			}
			else
			{
				console::write("/painttraverse/ current vguipanel_name => " + string(vguipanel_name), red);
				return;
			}
		}*/

		/*
		vguiPanel
		0x
		*/

		//cout << vguiPanel << endl;

		//game::interfaces::surface->set_color(0, 255, 0, 255);
		//game::interfaces::surface->filled_rect(1, 1, 100, 200);
	}

	// thx catalinadragan22 (c) // uc
	// TODO: изучить подробнее
	memory::vthook* netchannel;
	using send_datagram_fn = int(__thiscall*)(CNetChan*, /*void*,*/ void*);
	send_datagram_fn o_send_datagram;
	int __fastcall send_datagram_hook(CNetChan* netchannel, void*, void* datagram)
	{
		if (sets->misc.fake_ping == 0 || !_engine->in_game() /*|| datagram*/)
			return o_send_datagram(netchannel, datagram);

		auto instate = netchannel->m_nInReliableState;
		auto in_sequencenr = netchannel->m_nInSequenceNr;

		misc::fake_ping::start(netchannel);
		misc::fake_ping::end(netchannel);

		int ret = o_send_datagram(netchannel, datagram);

		netchannel->m_nInReliableState = instate;
		netchannel->m_nInSequenceNr = in_sequencenr;

		return ret;
	}

	memory::vthook* client;
	using create_move_fn = void(__stdcall*)(int sequence_number, float input_sample_frametime, bool active);
	create_move_fn o_create_move;
	void __stdcall create_move_hook(int sequence_number,			// sequence_number of this cmd
									float input_sample_frametime,	// Frametime for mouse input sampling
									bool active)
	{
		o_create_move(sequence_number, input_sample_frametime, active);

		if (!_input)
		{
			console::write("/fail/ get input", red);
			return;
		}

		//вызывает ошибку из-за слишком частых запросов
		//cusercmd* cmd = _input->get_usercmd(sequence_number);

		/*if (global::cmd && global::cmd->command_number != 0)
		{
			cout << "global::cmd : " << global::cmd << endl;
			//cout << "global::cmd->buttons : " << global::cmd->buttons << endl;
			//if (global::cmd->buttons & IN_ATTACK)
			//	cout << "shot" << endl;
		}*/

		DWORD* p_ebp;
		__asm mov p_ebp, ebp;
		byte* p_sendpacket = (byte*)(*p_ebp - 0x1);

		global::sendpacket = *p_sendpacket;

		global::cmd = &(*(cusercmd**)((DWORD)_input + USERCMDOFFSET))[sequence_number % MULTIPLAYER_BACKUP];
		c_verified_usercmd* verified_usercmd = &(*(c_verified_usercmd**)((DWORD)_input + VERIFIEDCMDOFFSET))[sequence_number % MULTIPLAYER_BACKUP];

		if (!sets->menu.panic && _engine->is_connected() && _engine->in_game() && global::cmd && global::cmd->command_number != 0 &&
			(global::local_id = _engine->get_local_id()) > 0 && (global::local = _ent_list->get_centity(global::local_id)))
		{
			if ((global::map_changed || sets->misc.fake_ping == 0) && netchannel)
			{
				netchannel->unhook();
				netchannel = null;
				o_send_datagram = null;
			}
			// TODO: ќЅЌќ¬»“№ » ¬≈–Ќ”“№
			/*else if (_clientstate && _clientstate->m_NetChannel && !netchannel)
			{
				console::write_hex("netchannel", (dword)_clientstate->m_NetChannel, darkgreen);
				netchannel = new memory::vthook((dword**)_clientstate->m_NetChannel);
				console::write_hex("/vthook/ netchannel", (dword)netchannel, darkgreen);
				o_send_datagram = (send_datagram_fn)netchannel->hook_function((dword)send_datagram_hook, 46);
				console::write_hex("/hook/ o_send_datagram", (dword)o_send_datagram, darkgreen);
			}*/

			// TODO: изучить подробнее
			/*if (_clientstate)
			{
				if (_clientstate->m_NetChannel && global::map_changed)
				{
					myfile << "_clientstate : " << (dword)_clientstate << endl;
					myfile << "netchannel : " << (dword)(_clientstate + 0x0010) << endl;
					netchannel = new memory::vthook((dword**)(_clientstate + 0x0010));
					myfile << "/hook/ netchannel : " << (dword)netchannel << endl;
					if (netchannel)
						o_send_datagram = (send_datagram_fn)netchannel->hook_function((dword)send_datagram_hook, 46);
					//console::write_hex("/hook/ o_send_datagram", (dword)o_send_datagram, darkgreen);
					myfile << "/hook/ o_send_datagram : " << (dword)o_send_datagram << endl;
				}
			}*/

			misc::nightmode();
			misc::remove();
			_globals = interfaces::pl_info_manager->get_globalvars();
			global::curtime = (float)global::local->get_tickbase() * _globals->interval_per_tick;
			global::weapon = global::local->get_weapon();
			auto observed = global::local->get_spec_player();
			global::local_observed = (global::local->valid() || observed == nullptr || global::local->get_spec_mode() != 4) ? global::local : observed;
			
			
			//obj::spectators::target = global::local->valid() ? global::local : global::local->get_spec_player();
			//auto p_weapon = global::local->get_weapon();
			//auto p_data = p_weapon->get_data();
			//cout << "p_data.name : " << p_data.szClassName << endl;
			//global::weapon = p_weapon;
			//if ((global::weapon = global::local->get_weapon()))
			//	global::wpn_data = global::weapon->get_data();
			//auto id = global::weapon->GetWeaponID();
			//cout << "weapon_id : " << id << endl;
			//cout << "name : " << global::local->get_weapon()->get_data().szClassName << endl;
			//cout << "curtime : " << global::curtime << endl;
			//cout << "next_prim_attack : " << global::weapon->next_primary_attack() << endl;

			//kolonote:
			//css fix for head triggering (bbox_maxs z component is too small)
			//credits: me, wav
			// (c) iwebz kolo
			// TODO: не работает, ломаетс€ виз чек, скорее всего - что-то устарело
			/*for (INT ax = 1; ax <= _engine->get_max_clients(); ax++)
			{
				centity* pBaseEntity = _ent_list->get_centity(ax);

				if (!pBaseEntity
					|| !pBaseEntity->valid()
					|| pBaseEntity == global::local)
					continue;

				PVOID pCollisionProperty = pBaseEntity->GetCollisionProperty();

				PFLOAT pfvecMaxsZ = (PFLOAT)((DWORD)pCollisionProperty + 0x1C);//vecMaxs.z
				PFLOAT pfvecMaxsZTwo = (PFLOAT)((DWORD)pCollisionProperty + 0x34);//vecMaxs2.z ???

				Vector vMini, vMaxi;
				pBaseEntity->get_render_bounds(vMini, vMaxi);

				if (*pfvecMaxsZ == vMaxi.z && *pfvecMaxsZTwo == vMaxi.z)
					continue;

				*pfvecMaxsZ = vMaxi.z;
				*pfvecMaxsZTwo = vMaxi.z;

				Vector vecSize;
				VectorSubtract(vMaxi, vMini, vecSize);
				float fNewRadius = vecSize.Length() * 0.5f;

				*(PFLOAT)((DWORD)pCollisionProperty + 0x38) = fNewRadius;//m_flRadius

				pBaseEntity->add_eflags(0x4000);
			}*/

			if ((sets->legit.enabled && (sets->legit.aim.fov > 0.f || sets->legit.backtrack.enabled || sets->legit.knifebot)) || sets->info.opened || sets->spec.opened || (sets->visuals.enabled && ((sets->visuals.esp_filter[0] && sets->visuals.esp_show[2]) || sets->visuals.crosshair == 2)))
			{
				legit::start();

				server::local.type = global::weapon->get_type();

				for (int id = 0; id < _engine->get_max_clients(); id++)
				{
					if (id == global::local_id)
					{
						//server::players[id].valid = false;
						continue;
					}

					centity* entity = _ent_list->get_centity(id);
					player_info_t pinfo;
					if (!entity || entity->is_dormant() || !_engine->get_playerinfo(id, &pinfo))
					{
						//server::players[id].valid = false;
						//ZeroMemory(legit::backtrack::records[id], sizeof(legit::backtrack::records[id]));
						continue;
					}

					legit::loop(id, entity);

					/*if (sets->visuals.enabled && sets->visuals.esp_filter[0] && sets->visuals.esp_show[3])
					{
						if (entity->valid())
						{
							auto weapon = entity->get_weapon();
							if (weapon)
							{
								auto type = weapon->get_type();
								if (type != weap_max)
								{
									if (sets->visuals.esp_bar[3]) server::players[id].clip = weapon->get_clip1();
									server::players[id].weapon = weapon->get_name();
									server::players[id].valid = true;
								}
								else
								{
									server::players[id].clip = -1;
									server::players[id].weapon = "none";
								}
							}
							else
							{
								server::players[id].clip = -1;
								server::players[id].weapon = "none";
							}
						}
					}*/

					/*server::max_players = id + 1;
					if (sets->info.opened)
					{
						auto weapon = entity->get_weapon();
						server::players[id].health = entity->get_hp();
						server::players[id].clip = weapon ? weapon->get_clip1() : -1;
						server::players[id].team = entity->get_team();
						server::players[id].lifestate = entity->get_life_state();
						server::players[id].weapon = weapon ? weapon->get_name() : "invalid";
					}
					server::players[id].name = pinfo.name;
					if (sets->spec.opened) server::players[id].spec_player = entity->get_spec_player();
					server::players[id].valid = true;*/
				}
			
				legit::end();
			}


			//auto cl_interp = _cvar->FindVar("cl_interp");
			//if (cl_interp->m_nValue > 5) _cvar->ConsoleColorPrintf(color(0, 255, 0), "cl_interp %i", cl_interp->m_nValue);
			//else _cvar->ConsoleColorPrintf(color(255, 0, 0), "cl_interp %i", cl_interp->m_nValue);
			//cout << "cl_interp " << cl_interp->m_fValue << endl;

			misc::run();
			misc::draw::clear(true);

			//auto p_weapon = global::local->get_weapon();
			//cout << "p_weapon : " << p_weapon << endl;
			//p_weapon->get_data();
			//cout << "p_data : " << *(dword*)&p_data << endl;
			//if (global::key[VK_DOWN])
			//{
				//auto p_data = p_weapon->get_data();
				//cout << "p_weapon->is_pistol() : " << p_weapon->is_pistol(p_data) << endl;
			//	cout << "////////////////\n_globals->curtime : " << _globals->curtime << endl;
			//	cout << "p_weapon->next_prim_attack() : " << p_weapon->next_primary_attack() << endl;
			//	cout << "p_weapon->next_sec_attack() : " << p_weapon->next_secondary_attack() << endl;
			//}

			//auto vm_handle = global::local->get_vm_handle();
			//cbaseviewmodel* viewmodel = (cbaseviewmodel*)vm_handle;
			//cout << "vm_handle : " << vm_handle << ", viewmodel : " << viewmodel << ", fov : " << global::local->get_fov() << endl;
			//if (sets->visuals.crosshair)
			//{
			//	global::local->get_fov() += 40;
				//viewmodel->CalcViewModelView(global::local, cvector(100, 100, 100), qangle(0, 0, 0));
			//}
		}
		else
		{
			misc::draw::clear(false);
		}

		if (!global::sendpacket)
		{
			global::chocked_packets++;
			if (global::chocked_packets > 14)
			{
				global::sendpacket = true;
				global::chocked_packets = 0;
			}
		}
		else global::chocked_packets = 0;

		verified_usercmd->m_cmd = *global::cmd;
		verified_usercmd->m_crc = global::cmd->GetChecksum();

		//cout << "sendpacket : " << (global::sendpacket ? "send" : "not send") << ", chocked_packets : " << global::chocked_packets << endl;

		*p_sendpacket = global::sendpacket;
	}

	/*using frame_stage_notify_fn = void(__stdcall*)(clientframestage_t stage);
	frame_stage_notify_fn o_frame_stage_notify;
	void __stdcall frame_stage_notify_hook(clientframestage_t stage)
	{
		static bool reset = true;
		if (sets->legit.enabled && sets->legit.backtrack && _engine->is_connected() && _engine->in_game() && global::local->valid() && (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START))
		{
			legit::backtrack::run(true);			
			if (!reset) reset = true;
		}
		else if (reset)
		{
			ZeroMemory(legit::backtrack::records, sizeof(legit::backtrack::records));
			legit::backtrack::best_record = legit::backtrack::crecord();
			legit::backtrack::best_fov = 9999.f;
			reset = false;
		}

		o_frame_stage_notify(stage);
	}

	using write_usercmd_delta_to_buf_fn = bool(__stdcall*)(dword* buf, int from, int to, bool isnewcommand);
	write_usercmd_delta_to_buf_fn o_write_usercmd_delta_to_buf;
	bool __stdcall write_usercmd_delta_to_buf_hook(dword* buf, int from, int to, bool isnewcommand)
	{
		//cout << "buf : " << buf << ", from : " << from << ", to : " << to << ", isnewcommand : " << isnewcommand << endl;
		//return _client->write_usercmd_delta_to_buf(buf, from, to, isnewcommand);
		return o_write_usercmd_delta_to_buf(buf, from, to, isnewcommand);
	}

	using render_view_fn = void(__stdcall*)(const cviewsetup & view, int nClearFlags, int whatToDraw);
	render_view_fn o_render_view;
	void __stdcall render_view_hook(const cviewsetup& view, int nClearFlags, int whatToDraw)
	{
		cout << "view.fov : " << view.fov << ", nClearFlags : " << nClearFlags << ", whatToDraw : " << whatToDraw << endl;

		o_render_view(view, nClearFlags, whatToDraw);
	}*/

	memory::vthook* input;
	using get_usercmd_fn = cusercmd*(__stdcall*)(int sequence_number);
	get_usercmd_fn o_get_usercmd;
	cusercmd* __stdcall get_usercmd_hook(int sequence_number)
	{
		//auto result = o_get_usercmd(sequence_number);
		//auto newcmd = &(*(cusercmd**)((DWORD)_input + USERCMDOFFSET))[sequence_number % MULTIPLAYER_BACKUP];
		//global::cmd = result;
		//cout << "get_usercmd_hook result : " << result << endl;
		//cout << "some shit cmd result : " << newcmd << endl;

		return &(*(cusercmd**)((DWORD)_input + USERCMDOFFSET))[sequence_number % MULTIPLAYER_BACKUP];
	}

	memory::vthook* model_render;
	using draw_model_execute_fn = void(__stdcall*)(const DrawModelState_t & state, const ModelRenderInfo_t & p_info, matrix3x4_t * p_custom_bone_to_world);
	draw_model_execute_fn o_draw_model_execute;
	void __stdcall draw_model_execute_hook(const DrawModelState_t& state, const ModelRenderInfo_t& p_info, matrix3x4_t* p_custom_bone_to_world)
	{
		model_render->unhook();

		//auto model_name = _model_info->get_model_name(p_info.pModel);
		//cout << p_info.entity_index << " " << model_name << endl;

		models::run(state, p_info, p_custom_bone_to_world);
		
		model_render->rehook();
	}

	memory::vthook* clientmode;
	/*using override_view_fn = void(__stdcall*)(cviewsetup* p_setup);
	override_view_fn o_override_view;
	void __stdcall override_view_hook(cviewsetup* p_setup) // почему-то не работает, пробовал все
	{
		cout << p_setup->fov << ", " << p_setup->fovViewmodel << endl;

		if (cvar(fov).value > 0)
			p_setup->fov = cvar(fov).value;

		if (cvar(vm_fov).value > 0)
			p_setup->fovViewmodel = cvar(vm_fov).value;

		if (cvar(fov).value == 0 && cvar(vm_fov).value == 0)
			o_override_view(p_setup);
	}*/

	using get_vm_fov_fn = float(__stdcall*)();
	get_vm_fov_fn o_get_vm_fov;
	float __stdcall get_vm_fov_hook()
	{
		return ((cvar(vm_fov).value > 0 && !sets->menu.panic) ? cvar(vm_fov).value : o_get_vm_fov());
	}

	memory::vthook* surface;
	using lock_cursor_fn = void(__fastcall*)(isurface*);
	lock_cursor_fn o_lock_cursor;
	void __fastcall lock_cursor_hook(isurface* surface, void*)
	{
		//surface->set_color(255, 255, 255, 255);
		//surface->filled_rect(0, 0, 100, 100);

		if (sets->menu.opened)
		{
			surface->unlock_cursor();//63
			//surface->set_cursor(dc_arrow);//set 54
			*global::lock_cursor = false;
		}
		else
		{
			*global::lock_cursor = true;
			//surface->set_cursor(cursor_code::dc_none);
			o_lock_cursor(surface);
		}
	}

	/*memory::vthook* engine_sound;
	using emit_sound_fn = void(__stdcall*)(irecipientfilter&, int, int, const char*, float, float, int, int, const Vector*, const Vector*, void*, bool, float, int);
	emit_sound_fn o_emit_sound;
	void __stdcall emit_sound_hook(irecipientfilter& filter, int entity_index, int channel, const char* sample,
		float volume, float attenuation, int flags = 0, int pitch = 100,
		const Vector* origin = NULL, const Vector* direction = NULL, void* shit = NULL, bool update_positions = true, float soundtime = 0.0f, int speakerentity = -1)
	{
		cout << "id : " << entity_index << ", channel : " << channel << ", name : " << sample << ", time : " << soundtime << endl;

		o_emit_sound(filter, entity_index, channel, sample, volume, attenuation, flags, pitch, origin, direction, shit, update_positions, soundtime, speakerentity);
	}*/

	memory::vthook* event_manager;
	using fire_event_clientside_fn = bool(__stdcall*)(igameevent*);
	fire_event_clientside_fn o_fire_event_clientside;
	bool __stdcall fire_event_clientside_hook(igameevent* p_event)
	{
		events::on_fire_event(p_event, p_event->get_name());

		return o_fire_event_clientside(p_event);
	}

	void do_them()
	{// старый		long -> wndproc			  наш wndproc	   окно cs:go	задать новый адрес					    новый адрес
		o_wndproc = reinterpret_cast<wndproc>(SetWindowLongPtr(global::window, GWLP_WNDPROC, reinterpret_cast<long>(wndproc_hook)));
		console::write_hex("/hook/ o_wndproc", (dword)o_wndproc, darkgreen);

		d3d9 = new memory::vthook((dword**)game::signatures::d3d9_device);
		console::write_hex("/vthook/ d3d9", (dword)d3d9, darkgreen);
		o_endscene = (endscene_fn)d3d9->hook_function((dword)endscene_hook, 42);
		console::write_hex("/hook/ o_endscene", (dword)o_endscene, darkgreen);
		o_reset = (reset_fn)d3d9->hook_function((dword)reset_hook, 16);
		console::write_hex("/hook/ o_reset", (dword)o_reset, darkgreen);

		panel = new memory::vthook((dword**)_panel);
		console::write_hex("/vthook/ panel", (dword)panel, darkgreen);
		o_painttraverse = (painttraverse_fn)panel->hook_function((dword)painttraverse_hook, 41);
		console::write_hex("/hook/ o_painttraverse", (dword)o_painttraverse, darkgreen);

		surface = new memory::vthook((dword**)_surface);
		console::write_hex("/vthook/ surface", (dword)surface, darkgreen);
		o_lock_cursor = (lock_cursor_fn)surface->hook_function((dword)lock_cursor_hook, 62);
		console::write_hex("/hook/ o_lock_cursor", (dword)o_lock_cursor, darkgreen);

		client = new memory::vthook((dword**)_client);
		console::write_hex("/vthook/ client", (dword)client, darkgreen);

		_input = (cinput*) * *(PDWORD*)(client->get_func_address(21) + INPUTOFFSET); // INPUTOFFSET
		console::write_hex("/interface/ input", (dword)_input, darkgreen);
		input = new memory::vthook((dword**)_input);
		console::write_hex("/vthook/ input", (dword)input, darkgreen);
		o_get_usercmd = (get_usercmd_fn)input->hook_function((dword)get_usercmd_hook, 8);
		console::write_hex("/hook/ o_get_usercmd", (dword)o_get_usercmd, darkgreen);

		o_create_move = (create_move_fn)client->hook_function((dword)create_move_hook, 21);
		console::write_hex("/hook/ o_create_move", (dword)o_create_move, darkgreen);
		// fsn рабочий
		//o_frame_stage_notify = (frame_stage_notify_fn)client->hook_function((dword)frame_stage_notify_hook, 35);
		//console::write_hex("/hook/ o_frame_stage_notify", (dword)o_frame_stage_notify, darkgreen);
		//o_write_usercmd_delta_to_buf = (write_usercmd_delta_to_buf_fn)client->hook_function((dword)write_usercmd_delta_to_buf_hook, 23);
		//console::write_hex("/hook/ o_write_usercmd_delta_to_buf", (dword)o_write_usercmd_delta_to_buf, darkgreen);
		//o_render_view = (render_view_fn)client->hook_function((dword)render_view_hook, 27);
		//console::write_hex("/hook/ o_render_view", (dword)o_render_view, darkgreen);

		model_render = new memory::vthook((dword**)_model_render);
		console::write_hex("/vthook/ model_render", (dword)model_render, darkgreen);
		o_draw_model_execute = (draw_model_execute_fn)model_render->hook_function((dword)draw_model_execute_hook, 19);
		console::write_hex("/hook/ o_draw_model_execute", (dword)o_draw_model_execute, darkgreen);

		clientmode = new memory::vthook((dword**)_clientmode);
		console::write_hex("/vthook/ clientmode", (dword)clientmode, darkgreen);
		//o_override_view = (override_view_fn)clientmode->hook_function((dword)override_view_hook, 16);
		//console::write_hex("/hook/ o_override_view", (dword)o_override_view, darkgreen);
		o_get_vm_fov = (get_vm_fov_fn)clientmode->hook_function((dword)get_vm_fov_hook, 32);
		console::write_hex("/hook/ o_get_vm_fov", (dword)o_get_vm_fov, darkgreen);

		//myfile << "_clientstate : " << hex << (dword)_clientstate << endl;
		//myfile << "netchannel : " << hex << (dword)(_clientstate->m_NetChannel) << endl;
		//netchannel = new memory::vthook((dword**)_clientstate->m_NetChannel);
		//myfile << "/hook/ netchannel : " << hex << (dword)netchannel << endl;
		//netchannel = new memory::vthook((dword**)(_clientstate + 0x0010));
		//myfile << "/hook/ netchannel : " << hex << (dword)netchannel << endl;
		//if (netchannel)
		//	o_send_datagram = (send_datagram_fn)netchannel->hook_function((dword)send_datagram_hook, 46);
		//console::write_hex("/hook/ o_send_datagram", (dword)o_send_datagram, darkgreen);
		//myfile << "/hook/ o_send_datagram : " << (dword)o_send_datagram << endl;

		//engine_sound = new memory::vthook((dword**)_engine_sound);
		//console::write_hex("/vthook/ engine_sound", (dword)engine_sound, darkgreen);
		//o_emit_sound = (emit_sound_fn)engine_sound->hook_function((dword)emit_sound_hook, 4);
		//console::write_hex("/hook/ o_emit_sound", (dword)o_emit_sound, darkgreen);

		event_manager = new memory::vthook((dword**)_event_manager);
		console::write_hex("/vthook/ event_manager", (dword)event_manager, darkgreen);
		o_fire_event_clientside = (fire_event_clientside_fn)event_manager->hook_function((dword)fire_event_clientside_hook, 10);
		console::write_hex("/hook/ o_fire_event", (dword)o_fire_event_clientside, darkgreen);
	}

	void remove()
	{
		console::write("removing wndproc_hook");
		SetWindowLongPtr(global::window, GWLP_WNDPROC, reinterpret_cast<long>(o_wndproc));

		console::write("removing other hooks");
		d3d9->unhook();
		panel->unhook();
		surface->unhook();
		client->unhook();
		input->unhook();
		model_render->unhook();
		clientmode->unhook();

		if (netchannel)
		{
			console::write("removing send_datagram_hook");
			netchannel->unhook();
		}		
	}
}