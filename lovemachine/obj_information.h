#pragma once
#include "cvars.h"
#include "obj'ects.h"

namespace obj
{
	namespace information
	{
		void on_draw()
		{
			if (!sets->info.opened) return;

			if (sets->menu.opened)
			{
				static int stdelta[2];

				if (!sets->spec.hovered && !sets->menu.hovered && sets->menu.cont_hovered == -1 && global::key[VK_LBUTTON] && in_range(sets->info.x, sets->info.y, global::mouse.x, global::mouse.y, sets->info.x + sets->info.mx, sets->info.y + sets->info.my))
				{
					sets->info.hovered = true;
					sets->info.x = global::mouse.x - stdelta[0];
					sets->info.y = global::mouse.y - stdelta[1];
				}
				else
				{
					sets->info.hovered = false;
					stdelta[0] = global::mouse.x - sets->info.x;
					stdelta[1] = global::mouse.y - sets->info.y;
				}
			}

			static auto size = font::size(font::cont, "information");

			sets->info.x = max(3, min(sets->info.x, global::screen.right - sets->info.mx - 3));
			sets->info.y = max(sets->info.y - 11 - (size.bottom / 2), min(sets->info.y, global::screen.bottom - sets->info.my - 3));

			// основа инфо бокса
			prim::filled_box(sets->info.x - 2, sets->info.y - 10 - (size.bottom / 2), sets->info.x + sets->info.mx + 2, sets->info.y + sets->info.my + 2, color::background());
			prim::bordered_box(sets->info.x - 1, sets->info.y - 1, sets->info.x + sets->info.mx + 1, sets->info.y + sets->info.my + 1, color::outline());
			prim::bordered_box(sets->info.x - 3, sets->info.y - 11 - (size.bottom / 2), sets->info.x + sets->info.mx + 3, sets->info.y + sets->info.my + 3, color::outline());
			// основа инфо бокса

			font::draw(font::cont, sets->info.x + (sets->info.mx / 2), sets->info.y - 10, color::enabled() + 65, DT_CENTER | DT_VCENTER, "information");
			prim::filled_box(sets->info.x - 2, sets->info.y - 4, sets->info.x + sets->info.mx + 2, sets->info.y - 1, color::enabled());

			if (!_engine->is_connected() || !_engine->in_game() || !global::cmd || global::cmd->command_number == 0 || !global::local)
			{
				sets->info.mx = 200;
				sets->info.my = 40;
				return;
			}

			if ((sets->info.style[1] || sets->info.style[2]) && (sets->info.style[3] || sets->info.style[4] || sets->info.style[5]))
			{
				int pos_y = 1;

				for (int id = 0; id < server::max_players; id++)
				{
					//centity* entity = _ent_list->get_centity(id);
					
					//player_info_t pinfo;
					//if (!entity || !_engine->get_playerinfo(id, &pinfo))
					//	continue;

					//if (!server::players[id].valid) continue;

					string sinfo = "";

					/*if (server::players[id].team != 2 && server::players[id].team != 3)
					{
						sinfo = server::players[id].name.substr(0, 24) + " | spectator";
					}
					else*/ /*if ((sets->info.style[1] && server::players[id].team != global::local->get_team()) || (sets->info.style[2] && server::players[id].team == global::local->get_team()))
					{
						sinfo = server::players[id].name.substr(0, 24);

						if (sets->info.style[3] || sets->info.style[4])
						{
							if (sets->info.style[3]) sinfo += " | " + server::players[id].weapon;

							if (sets->info.style[4]) sinfo += " | " + to_str(server::players[id].clip);
						}

						if (sets->info.style[5])
						{
							if (server::players[id].health <= 0 || server::players[id].lifestate != life_alive)
								sinfo += " | dead";
							else sinfo += " | " + to_str(server::players[id].health);
						}

						//std::wstring wsinfo(sinfo.size() + 1, L'#');
						//mbstowcs(&wsinfo[0], sinfo.c_str(), sinfo.size() + 1);
					}*/
				
					if (!sinfo.empty())
					{
						int wchars_num = MultiByteToWideChar(CP_UTF8, 0, sinfo.c_str(), -1, NULL, 0);
						wchar_t* wstr = new wchar_t[wchars_num];
						MultiByteToWideChar(CP_UTF8, 0, sinfo.c_str(), -1, wstr, wchars_num);

						auto p_size = font::size_w(font::cont, wstr, wchars_num);

						sets->info.mx = max(200, p_size.right + 10);
						sets->info.my = pos_y + p_size.bottom + 4;

						font::draw_w(font::cont, sets->info.x + (sets->info.mx / 2) - (p_size.right / 2), sets->info.y + pos_y, color::ptext(), NULL, wstr);

						delete wstr;

						pos_y += p_size.bottom;
					}
				}
			}
		}
	}
}