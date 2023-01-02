#pragma once
#include "obj'ects.h"
#include "game shit.h"
#include "global.h"
#include "game classes.h"

namespace obj
{
	namespace spectators
	{
		centity* target = nullptr;

		void on_draw()
		{
			if (!sets->spec.opened) return;

			if (sets->menu.opened)
			{
				static int stdelta[2];

				if (!sets->info.hovered && !sets->menu.hovered && sets->menu.cont_hovered == -1 && global::key[VK_LBUTTON] && in_range(sets->spec.x, sets->spec.y, global::mouse.x, global::mouse.y, sets->spec.x + sets->spec.mx, sets->spec.y + sets->spec.my))
				{
					sets->spec.hovered = true;
					sets->spec.x = global::mouse.x - stdelta[0];
					sets->spec.y = global::mouse.y - stdelta[1];
				}
				else
				{
					sets->spec.hovered = false;
					stdelta[0] = global::mouse.x - sets->spec.x;
					stdelta[1] = global::mouse.y - sets->spec.y;
				}
			}

			static auto size = font::size(font::cont, "spectators");

			sets->spec.x = max(3, min(sets->spec.x, global::screen.right - sets->spec.mx - 3));
			sets->spec.y = max(sets->spec.y - 11 - (size.bottom / 2), min(sets->spec.y, global::screen.bottom - sets->spec.my - 3));

			// основа спек листа
			prim::filled_box(sets->spec.x - 2, sets->spec.y - 10 - (size.bottom / 2), sets->spec.x + sets->spec.mx + 2, sets->spec.y + sets->spec.my + 2, color::background());
			prim::bordered_box(sets->spec.x - 1, sets->spec.y - 1, sets->spec.x + sets->spec.mx + 1, sets->spec.y + sets->spec.my + 1, color::outline());
			prim::bordered_box(sets->spec.x - 3, sets->spec.y - 11 - (size.bottom / 2), sets->spec.x + sets->spec.mx + 3, sets->spec.y + sets->spec.my + 3, color::outline());
			// основа спек листа

			font::draw(font::cont, sets->spec.x + (sets->spec.mx / 2), sets->spec.y - 10, color::enabled() + 65, DT_CENTER | DT_VCENTER, "spectators");
			prim::filled_box(sets->spec.x - 2, sets->spec.y - 4, sets->spec.x + sets->spec.mx + 2, sets->spec.y - 1, color::enabled());

			if (!_engine->is_connected() || !_engine->in_game() || !global::cmd || global::cmd->command_number == 0 || !global::local)
			{
				sets->spec.mx = 150;
				sets->spec.my = 30;
				return;
			}

			if (target)
			{
				int pos_y = 1;

				/*for (int id = 0; id < server::max_players; id++)
				{
					//centity* entity = _ent_list->get_centity(id);
					//player_info_t pinfo;
					//if (!entity || entity->get_life_state() == life_alive || entity->get_spec_player() != target || !_engine->get_playerinfo(id, &pinfo)) 
					//	continue;

					if (!server::players[id].valid || server::players[id].lifestate == life_alive || server::players[id].spec_player != target)
						continue;
					
					int wchars_num = MultiByteToWideChar(CP_UTF8, 0, server::players[id].name.c_str(), -1, NULL, 0);
					wchar_t* wstr = new wchar_t[wchars_num];
					MultiByteToWideChar(CP_UTF8, 0, server::players[id].name.c_str(), -1, wstr, wchars_num);

					auto p_size = font::size_w(font::cont, wstr, wchars_num);

					sets->spec.mx = max(150, p_size.right);
					sets->spec.my = pos_y + p_size.bottom + 4;

					font::draw_w(font::cont, sets->spec.x + (sets->spec.mx / 2) - (p_size.right / 2), sets->spec.y + pos_y, color::ptext(), NULL, wstr);

					delete wstr;

					pos_y += p_size.bottom;
				}*/
			}
		}
	}
}