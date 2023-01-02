#pragma once
#include "game shit.h"
#include "game classes.h"
#include "game def's.h"
#include "settings.h"
#include "net shit.h"
#include "draw.h"

//ofstream myfile;

namespace misc
{
	namespace fake_ping
	{
		struct state_t
		{
			state_t(int in_seq, int in_rel, float time)
			{
				this->in_seq = in_seq;
				this->in_rel = in_rel;
				this->time = time;
			}

			int in_seq, in_rel;
			float time;
		};

		deque<state_t> states;

		void start(CNetChan* netchannel)
		{
			states.push_front({ netchannel->m_nInSequenceNr, netchannel->m_nInReliableState, _globals->realtime });
			// 2048????
			if (states.size() > 2048) states.pop_back();
		}

		void end(CNetChan* netchannel)
		{
			for (auto state : states)
			{
				//cout << "realtime: " << _globals->realtime << ", state.time: " << state.time << ", delta: " << (_globals->realtime - state.time)  << endl;
				if ((_globals->realtime - state.time) > (float)((float)sets->misc.fake_ping / 1000.f))
				{
					netchannel->m_nInSequenceNr = state.in_seq;
					netchannel->m_nInReliableState = state.in_rel;
					break; // ???
				}
			}
		}
	}

	void autopistol()
	{
		/*if (cvar(antismac).value)
		{
			return;
			if (!global::weapon->is_pistol())
				return;

			static bool last_shot = false;
			static bool should_fake = false;
			static bool disable = false;

			if (!last_shot && should_fake)
			{
				should_fake = false;
				disable = true;
				_engine->clientcmd_unrestricted("+attack");
			}
			else if (global::cmd->buttons & IN_ATTACK && global::key[VK_LBUTTON])
			{
				if (global::curtime >= global::weapon->next_primary_attack() &&
					(cvar(ap_percent).value >= 97 || ((rand() % 100) >= (100 - cvar(ap_percent).value))))
				{
					last_shot = true;
					should_fake = true;
				}
				else
				{
					global::cmd->buttons &= ~IN_ATTACK;
					last_shot = false;
				}

				disable = true;
			}
			else if (disable)
			{
				last_shot = false;
				should_fake = false;
				disable = false;
				_engine->clientcmd_unrestricted("-attack");
			}
		}
		else
		{*/

		static bool undo = false;

		if (!(global::cmd->buttons & IN_ATTACK) /*|| !global::key[VK_LBUTTON]*/ || !global::weapon || server::local.type != weap_pistol || global::curtime >= global::weapon->next_primary_attack() || (cvar(antismac).value && undo))
		{
			//if (undo)
			//{
			//	send_mouse(MOUSEEVENTF_RIGHTUP);
			//	undo = false;
			//}
			return;
		}

		//mouse_input(MOUSEEVENTF_LEFTUP);
		const auto id = global::weapon->get_weaponid();
		if (cvar(antismac).value /*&& id != weapon_usp && id != weapon_glock*/)
		{
			//unpress_left_mouse_key();

			//send_key(VK_LBUTTON, false);
			//send_mouse(MOUSEEVENTF_RIGHTDOWN);
			//undo = true;

			//if (global::weapon->get_weaponid() != weapon_usp && global::weapon->get_weaponid() != weapon_glock)
				//send_key(VK_RBUTTON, true);
		}
		else
		{
			global::cmd->buttons &= ~IN_ATTACK;
		}

		//global::cmd->command_number -= 1;
		//global::cmd->buttons &= ~IN_ATTACK;
		//}
	}

	void autostrafer()
	{
		if ((global::local->get_flags() & FL_ONGROUND) || global::cmd->mousedx == 0)
			return;

		if (cvar(antismac).value)
		{
			return;
		}

		global::cmd->sidemove = global::cmd->mousedx < 0 ? -400 : 400;

		//if (!cvar(antismac).value)
		/*{
			//static bool disable = false;
			if ((global::local->get_flags() & FL_ONGROUND) || !global::key[VK_SPACE] || global::cmd->mousedy == 0.f)
			{
				//if (disable)
				//{
				//	_engine->clientcmd_unrestricted("-forward");
				//	_engine->clientcmd_unrestricted("-back");
				//	_engine->clientcmd_unrestricted("-moveleft");
				//	_engine->clientcmd_unrestricted("-moveright");
				
				//	disable = false;
				//}
				return;
			}

			if (cvar(antismac).value)
			{
				global::cmd->mousedy < 0 ? send_key(0x41, true) : send_key(0x44, true);
				global::cmd->mousedy < 0 ? send_key(0x44, false) : send_key(0x41, false);
			}
			else global::cmd->sidemove = global::cmd->mousedy < 0.f ? -400 : 400;*/

			/*if (global::key[0x41]) // A
			{
				if (global::cmd->buttons & IN_MOVELEFT)
				{
					global::cmd->command_number -= 1;
					global::cmd->buttons &= ~IN_MOVELEFT;
				}
				if (global::cmd->mousedx < -1) send_key(0x53, true);//_engine->clientcmd_unrestricted("+back");
				else if (global::cmd->mousedx > 1) send_key(0x57, true); //_engine->clientcmd_unrestricted("+forward");

				//disable = true;
			}
			else if (global::key[0x53]) // S // не идеально
			{
				if (global::cmd->buttons & IN_BACK)
				{
					global::cmd->command_number -= 1;
					global::cmd->buttons &= ~IN_BACK;
				}
				if (global::cmd->mousedx < -1) send_key(0x44, true);//_engine->clientcmd_unrestricted("+moveright");
				else if (global::cmd->mousedx > 1) send_key(0x41, true);//_engine->clientcmd_unrestricted("+moveleft");

				//disable = true;
			}
			else if (global::key[0x44]) // D // не идеально
			{
				if (global::cmd->buttons & IN_MOVERIGHT)
				{
					global::cmd->command_number -= 1;
					global::cmd->buttons &= ~IN_MOVERIGHT;
				}
				if (global::cmd->mousedx < -1) send_key(0x57, true);//_engine->clientcmd_unrestricted("+forward");
				else if (global::cmd->mousedx > 1) send_key(0x53, true);//_engine->clientcmd_unrestricted("+back");

				//disable = true;
			}
			else // W или по дефолту вперед
			{
				//if (global::key[0x57]) _engine->clientcmd_unrestricted("-forward");
				if (global::cmd->buttons & IN_FORWARD)
				{
					global::cmd->command_number -= 1;
					global::cmd->buttons &= ~IN_FORWARD;
				}
				if (global::cmd->mousedx < -1) send_key(0x41, true);//_engine->clientcmd_unrestricted("+moveleft");
				else if (global::cmd->mousedx > 1) send_key(0x44, true);//_engine->clientcmd_unrestricted("+moveright");

				//disable = true;
			}*/
		//}
	}

	void autojump()
	{
		static bool last_jumped = false;
		static bool should_fake = false;
		static bool disable = false;

		if (!last_jumped && should_fake)
		{
			should_fake = false;
			disable = true;
			//global::cmd->buttons |= IN_JUMP;
			_engine->clientcmd_unrestricted("+jump");
		}
		else if (global::cmd->buttons & IN_JUMP && global::key[VK_SPACE])
		{
			if ((global::local->get_flags() & FL_ONGROUND) && 
				(sets->misc.aj_percent >= 97 || ((rand() % 100) > (100 - sets->misc.aj_percent))))
			{
				last_jumped = true;
				should_fake = true;
			}
			else
			{
				global::cmd->buttons &= ~IN_JUMP;
				last_jumped = false;
			}

			disable = true;
		}
		else if (disable)
		{
			last_jumped = false;
			should_fake = false;
			disable = false;
			_engine->clientcmd_unrestricted("-jump");
		}
	}

	inline bool shooting()
	{
		if ((global::weapon && global::weapon->get_weaponid() != weapon_none) &&
			((global::cmd->buttons & IN_ATTACK && global::weapon->next_primary_attack() <= global::curtime) ||
			(global::weapon->get_weaponid() == weapon_knife && global::cmd->buttons & IN_ATTACK2 && global::weapon->next_secondary_attack() <= global::curtime)))
			return true;

		return false;
	}

	void lag()
	{
		static int lag_tick = 0;
		static int max_tick = sets->misc.lag_factor >= 2 ? rand() % sets->misc.lag_factor : 1;

		if (shooting() || /*lag_tick >= sets->misc.lag_factor*/ lag_tick >= max_tick /*|| (sets->misc.lag_mode == 1 && rand() % 2 == 0)*/)
		{
			lag_tick = 0;
			max_tick = sets->misc.lag_mode == 0 ? sets->misc.lag_factor : sets->misc.lag_factor >= 2 ? rand() % sets->misc.lag_factor : 1;
			global::sendpacket = true;
			return;
		}

		if (lag_tick < sets->misc.lag_factor)
		{
			global::sendpacket = false;
			lag_tick++;
		}
	}

	void nightmode()
	{
		static int st_value = cvar(cvar_ids::nightmode).value;
		float value;

		if (st_value != cvar(cvar_ids::nightmode).value || global::map_changed)
		{
			_engine->clientcmd_unrestricted("r_DrawSpecificStaticProp 0"); // :thinking:			

			for (auto handle = _mat_sys->first_material(); handle != _mat_sys->invalid_material(); handle = _mat_sys->next_material(handle))
			{
				auto material = _mat_sys->get_material(handle);
				auto name = material->get_name();
				auto group = material->get_texture_group_name();

				//myfile << name << " ||| " << group << endl;

				if (!strstr(group, "World") && !strstr(group, "Model") && !strstr(group, "SkyBox") && !strstr(group, "ClientEffect"))
					continue;

				value = 1.f - (cvar(cvar_ids::nightmode).value / 100.f);

				if (strstr(group, "World")) value = min(1.f, (value + 0.15f));
				else if (strstr(group, "Model"))
				{
					if (strstr(name, "player")) value = min(1.f, (value + 0.7f));
					else value = min(1.f, (value + 0.15f));
				}
				else if (strstr(group, "SkyBox")) value = min(1.f, (value + 0.5f));
				else if (strstr(group, "ClientEffect")) value = min(1.f, (value + 0.7f));
					
				material->colour_modulate(value, value, value);
			}

			st_value = cvar(cvar_ids::nightmode).value;
		}
	}

	void remove()
	{
		static bool st_remove[2] = { false, false };

		if (st_remove[0] != sets->visuals.remove[0] || global::map_changed)
		{
			//auto material = _mat_sys->find_material(/*"particle/smokesprites_0015"*//*"particle/smokesprites_0015"*/"particle/smoke1/smoke1_nearcull2", "Other textures");
			//auto material2 = _mat_sys->find_material(/*"particle/smoke1/smoke1"*/"particle/vistasmokev1/vistasmokev1_nearcull", "Other textures");
			auto material3 = _mat_sys->find_material("particle/smokesprites_0001"/*"particle/vistasmokev1/vistasmokev1_nearcull"*/, "ClientEffect textures");
			auto material4 = _mat_sys->find_material("particle/particle_smokegrenade", "ClientEffect textures");
			auto material5 = _mat_sys->find_material("particle/particle_smokegrenade1", "ClientEffect textures");

			if (sets->visuals.remove[0])
			{
				//material->set_materialvar_flag(MATERIAL_VAR_NO_DRAW, true);
				//material2->set_materialvar_flag(MATERIAL_VAR_NO_DRAW, true);
				material3->set_materialvar_flag(MATERIAL_VAR_NO_DRAW, true);
				material4->set_materialvar_flag(MATERIAL_VAR_NO_DRAW, true);
				material5->set_materialvar_flag(MATERIAL_VAR_NO_DRAW, true);
			}
			else
			{
				//material->set_materialvar_flag(MATERIAL_VAR_NO_DRAW, false);
				//material2->set_materialvar_flag(MATERIAL_VAR_NO_DRAW, false);
				material3->set_materialvar_flag(MATERIAL_VAR_NO_DRAW, false);
				material4->set_materialvar_flag(MATERIAL_VAR_NO_DRAW, false);
				material5->set_materialvar_flag(MATERIAL_VAR_NO_DRAW, false);
			}

			st_remove[0] = sets->visuals.remove[0];
		}
		
		if (st_remove[1] != sets->visuals.remove[1] || global::map_changed)
		{
			auto material = _mat_sys->find_material("effects/flashbang", "ClientEffect textures");
			auto material2 = _mat_sys->find_material("effects/flashbang_white", "ClientEffect textures");

			if (sets->visuals.remove[1])
			{
				material->set_materialvar_flag(MATERIAL_VAR_NO_DRAW, true);
				material2->set_materialvar_flag(MATERIAL_VAR_NO_DRAW, true);
			}
			else
			{
				material->set_materialvar_flag(MATERIAL_VAR_NO_DRAW, false);
				material2->set_materialvar_flag(MATERIAL_VAR_NO_DRAW, false);
			}

			st_remove[1] = sets->visuals.remove[1];
		}

		global::map_changed = false;
	}

	void run()
	{
		if (!global::local->valid()) return;

		if (sets->misc.fl_spam_always || sets->misc.fl_spam.is() == bind_true)
			_engine->clientcmd_unrestricted("impulse 100");

		bool lag_bind = sets->misc.lag_spam.is();
		if (sets->misc.lag_factor > 0 && (lag_bind == bind_no_key || lag_bind == bind_true))
			lag();
		else
			global::sendpacket = true;

		if (!*global::lock_cursor) return;

		if (sets->misc.autopistol) autopistol();

		if (sets->misc.autojump) autojump();

		if (sets->misc.autostrafer) autostrafer();
		
		if ((sets->misc.airstuck.is() == bind_true ||
			(sets->misc.slowmotion.is() == bind_true && ((GetTickCount64() % sets->misc.sm_speed) == 0))) &&
			!shooting()) 
			global::cmd->tick_count += 200;

		misc::draw::run();

		nightmode();
	}
}