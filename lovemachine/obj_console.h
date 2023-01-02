#pragma once
#include "cvars.h"
#include "obj'ects.h"
#include "configs.h"
#include "surface.h"

namespace obj
{
	namespace con
	{
		int history_id = -1;
		vector<string> history;

		void do_analysis(string text)
		{
			history.push_back(text);
			history_id = -1;

			if (text.substr(0, 5) == "echo ")
				console::write(text.substr(5), color::ptext());
			else if (strstr(text.c_str(), "panic"))
				sets->menu.panic = true;
			else if (text.substr(0, 5) == "load ")
			{
				if (configs::parse(text.substr(5)))
					console::write("config loaded succesfully", color(0, 200, 0));
				else
					console::write("something went wrong, try again", color(255, 0, 0));
			}
			else if (text.substr(0, 5) == "save ")
			{
				if (configs::write(text.substr(5)))
					console::write("config saved succesfully", color(0, 200, 0));
				else
					console::write("something went wrong, try again", color(255, 0, 0));
			}
			else if (text.substr(0, 11) == "config_list")
			{
				auto configs = configs::parse_configs();
				if (configs.empty()) console::write("no configs was found", color::ptext());
				else
				{
					console::write("/// configs list start ///", color(0, 200, 0));

					string output = "";
					for (int id = 0; id < configs.size(); id++)
					{
						auto config = configs.at(id);

						if (output.empty() || font::size(font::cont, (output + ", " + config).c_str()).right < (obj::menu_x - 5))
						{
							output += config;
							if (id == (configs.size() - 1))
								console::write(output, color::ptext());
							else output += ", ";

						}
						else
						{
							console::write(output, color::ptext());
							output = str(config) + ", ";
						}
					}

					console::write("/// configs list end ///", color(0, 200, 0));
				}
			}
			else if (text.substr(0, 11) == "reset_fonts")
			{
				surf::font::setup(surf::font::esp, "Gotham Pro Medium", 17, fw_normal, ff_antialias | ff_dropshadow);
			}
			else if (text.substr(0, 5) == "quit" || text.substr(0, 5) == "exit" || text.substr(0, 7) == "unhook" || text.substr(0, 7) == "unload")
			{
				global::unhook = true;
			}
			else
			{
				int start_point = -1, last_point = -1;
				for (int sid = 0; sid < text.size(); sid++)
				{
					if (text.at(sid) == ' ')
					{
						if (start_point == -1) start_point = sid;

						last_point = sid + 1;
					}
				}

				string cvar_name = start_point == -1 ? text : text.substr(0, start_point);
				string svalue = last_point != -1 ? text.substr(last_point, text.size()) : "no value";
				
				int id = cvars.find(cvar_name);
				if (id == -1 || cvar(id).type == -1)
				{
					console::write("no cvar \"" + text + "\"", color(200, 0, 0));
					return;
				}

				auto p_cvar = &cvar(id);

				if (start_point == -1 || svalue.empty() || (((int)svalue.at(0)) < 48 && ((int)svalue.at(0)) > 57))
				{
					if (p_cvar->type == cvar_bool)
						console::write(p_cvar->name + " (" + (p_cvar->value > 0.f ? "true" : "false") + ")" + p_cvar->comment, color::ptext());
					else
						console::write(p_cvar->name + " (" + to_str((int)p_cvar->value) + ")" + p_cvar->comment, color::ptext());
				}
				else
				{
					float new_value = stof(svalue);
					new_value = min(max(p_cvar->min, new_value), p_cvar->max);
					if (p_cvar->type == cvar_bool)
						console::write(cvar_name + " | " + (p_cvar->value > 0.f ? "true" : "false") + " -> " + (new_value > 0.f ? "true" : "false"), color(0, 255, 0));
					else
						console::write(cvar_name + " | " + to_str((int)p_cvar->value) + " -> " + to_str((int)new_value), color(0, 255, 0));
					p_cvar->value = new_value;
				}
			}
		}

		//TODO : сделать взаимодействие мышкой
		bool textgetter(const char* name, string& text, int p_x, int p_y, int width, int height)
		{
			cont++;

			color background = color::disabled();
			color ctext = color::ptext();

			int x = sets->menu.x + p_x;
			int x1 = x + width;
			int y = sets->menu.y + p_y;
			int y1 = y + height;

			if ((sets->menu.cont_hovered == -1 || sets->menu.cont_hovered == cont) && 
				in_range(x, y, global::mouse.x, global::mouse.y, x1, y1))
			{
				background += 10;
				ctext += 20;

				if (global::key_click[VK_LBUTTON] && sets->menu.cont_hovered == -1)
				{
					background += 5;
					ctext += 10;
					sets->menu.cont_hovered = cont;
				}
			}
			else if (sets->menu.cont_hovered == cont && global::key_click[VK_LBUTTON])
			{
				sets->menu.cont_hovered = -1;
			}

			if (sets->menu.cont_hovered == cont)
			{
				background += 20;
				ctext += 30;

				if (!history.empty() && (global::key_click[VK_UP] || global::key_click[VK_DOWN]))
				{
					if (global::key_click[VK_UP]) history_id++;
					else if (global::key_click[VK_DOWN]) history_id--;

					if (history_id >= 0 && history_id < history.size())
					{
						text = history.at(history_id);
					}
					else
					{
						//history_id = history_id >= history.size() ? -1 : history_id < -1 ? history.size() - 1 : history_id;
						history_id = history_id < -1 ? history.size() - 1 : history_id >= history.size() ? -1 : history_id;
						if (history_id >= 0 && history_id < history.size())
						{
							text = history.at(history_id);
						}
						else
						{
							text = "";
						}
					}
				}

				if (!text.empty())
				{
					if (global::key_click[VK_RETURN])
					{
						do_analysis(text);
						text = "";
						//sets->menu.cont_hovered = -1;
					}
					else if (global::key_click[VK_BACK])
					{
						text.erase(text.begin() + text.size() - 1);
					}
					else if (global::key_click[VK_ESCAPE])
					{
						sets->menu.cont_hovered = -1;
					}
					else if ((global::key[VK_SHIFT] || global::key[VK_LSHIFT] || global::key[VK_RSHIFT]) && (global::key_click[VK_SUBTRACT] || global::key_click[VK_OEM_MINUS]))
					{
						if (font::size(font::cont, (text + '_').c_str()).right < (width - 10)) text += "_";
					}
				}

				for (int key_id = 0; key_id < 254; key_id++)
				{
					if (inputkeynames[key_id] != null && global::key_click[key_id] && font::size(font::cont, (text + inputkeynames[key_id]).c_str()).right < (width - 10))
					{
						text += inputkeynames[key_id];
					}
				}
			}

			prim::bordered_box(x - 1, y - 1, x1 + 1, y1 + 1, color::outline());
			prim::filled_box(x, y, x1, y1, background);
			font::draw(font::cont, x + (width / 2), y + (height / 2), ctext, DT_CENTER | DT_VCENTER, text.c_str());
			font::draw(font::cont, x1 + 5, y + (height / 2), ctext, DT_VCENTER, name);

			return false;
		}
	}
}