#pragma once
#include "includes.h"
#include "definitions.h"
#include "d3d.h"
#include "color.h"
#include "settings.h"
#include "global.h"
#include "console.h"

using namespace d3d;

namespace obj
{
	enum e_tab : int
	{
		legit,
		visuals,
		misc,
		tab_max
	};

	int page[tab_max];
	const char* name[tab_max] = { "legit", "visuals", "misc" };
	const char* pages_legit[3] = { "main", "aimbot", "trigger" };
	const char* pages_visuals[2] = { "main", "misc" };
	const char* pages_misc[5] = { "main", "binds", "chams_colors", "esp_colors", "draw" };
	const char* hitbox[5] = { "head", "chest", "pelvis", "arms", "legs"};
	const char* backtrack_style[4] = { "chams", "dots", "best", "last" };
	const char* chams[3] = { "disabled", "default", "flat" };
	const char* chams_style[3] = { "invis", "shine", "asus" };
	const char* other_chams[3] = { "weapon", "dropped", "asus_props" };
	const char* other_style[5] = { "chams", "color", "invis", "shine", "asus" };
	const char* esp_filter[6] = { "player", "weapon", "drop_nade", "throw_nade", "bomb", "defusers" };
	const char* esp_show[4] = { "name", "box", "weapon", "line" };
	const char* esp_bar[2] = { "health", "armor"/*, "pl_ammo", "drop_ammo"*/ };
	const char* esp_check[2] = { "invis", "sound" };
	const char* crosshair[3] = { "none", "always", "sniper only" };
	const char* info_style[5] = { "ip", "enemies", "friends", "weapon", /*"ammo",*/ "hp" };
	const char* ak47_skin[4] = { "default", "adidas", "bloodsport", "neon_rider" };
	const char* deagle_skin[2] = { "default", "hello_kitty" };
	const char* knife_skin[3] = { "default", "bowie", "karambit" };
	const char* other_skins[2] = { "c4", "grenades" };
	const char* remove[2] = { "smoke", "flash" };
	const char* lag_mode[2] = { "default", "random" };
	const char* draw_mode[2] = { "paint", "beam" };

	int cur_x = 5, cur_y = 40, const menu_x = 360, const menu_y = 184, cont = tab_max;

	inline bool in_range(int x, int y, int i, int z, int x1, int y1)
	{
		return (i >= x && i <= x1 && z >= y && z <= y1);
	}

	void on_open()
	{
		static int stdelta[2];

		if (!sets->info.hovered && sets->menu.cont_hovered == -1 && global::key[VK_LBUTTON] && in_range(sets->menu.x, sets->menu.y, global::mouse.x, global::mouse.y, sets->menu.x + menu_x, sets->menu.y + menu_y))
		{
			sets->menu.hovered = true;
			sets->menu.x = global::mouse.x - stdelta[0];
			sets->menu.y = global::mouse.y - stdelta[1];
		}
		else
		{
			sets->menu.hovered = false;
			stdelta[0] = global::mouse.x - sets->menu.x;
			stdelta[1] = global::mouse.y - sets->menu.y;
		}

		static auto msize = font::size(font::cont, "main");
		static auto csize = font::size(font::cont, "console");

		sets->menu.x = max(3, min(sets->menu.x, global::screen.right - menu_x - 3));
		sets->menu.y = max(/*sets->menu.y -*/ 11 + (csize.bottom / 2), min(sets->menu.y, global::screen.bottom - menu_y - 3));

		// основа меню
		prim::filled_box(sets->menu.x - 2, sets->menu.y - 10 - (csize.bottom / 2), sets->menu.x + menu_x + 2, sets->menu.y + menu_y + 2, color::background());
		prim::bordered_box(sets->menu.x - 1, sets->menu.y - 1, sets->menu.x + menu_x + 1, sets->menu.y + menu_y + 1, color::outline());
		prim::bordered_box(sets->menu.x - 3, sets->menu.y - 11 - (csize.bottom / 2), sets->menu.x + menu_x + 3, sets->menu.y + menu_y + 3, color::outline());
		// основа меню

		static int cur_tab_x = 0;

		color cmain = !sets->menu.console ? color::enabled() + 65 : color::ptext() - 70;
		color cconsole = sets->menu.console ? color::enabled() + 65 : color::ptext() - 70;

		if (in_range(sets->menu.x - 1, sets->menu.y - 16, global::mouse.x, global::mouse.y, sets->menu.x + (menu_x / 2), sets->menu.y - 1))
		{
			cmain += !sets->menu.console ? 20 : 50;

			if (global::key_click[VK_LBUTTON])
			{
				cmain += 10;
				sets->menu.cont_hovered = -1;
				sets->menu.console = false;
			}
		}
		else if (in_range(sets->menu.x + (menu_x / 2), sets->menu.y - 16, global::mouse.x, global::mouse.y, sets->menu.x + menu_x + 1, sets->menu.y - 1))
		{
			cconsole += sets->menu.console ? 20 : 50;

			if (global::key_click[VK_LBUTTON])
			{
				cconsole += 10;
				sets->menu.cont_hovered = -1;
				sets->menu.console = true;
			}
		}

		if (!sets->menu.console && cur_tab_x > 0) cur_tab_x -= 2;

		if (sets->menu.console && cur_tab_x < (menu_x / 2)) cur_tab_x += 2;

		cur_tab_x = max(0, min(cur_tab_x, (menu_x / 2)));

		font::draw(font::cont, sets->menu.x + (menu_x / 4), sets->menu.y - 10, cmain, DT_CENTER | DT_VCENTER, "main");
		font::draw(font::cont, sets->menu.x + ((menu_x / 4) * 3) + 1, sets->menu.y - 10, cconsole, DT_CENTER | DT_VCENTER, "console");
		prim::filled_box(sets->menu.x + cur_tab_x - 2, sets->menu.y - 4, sets->menu.x + cur_tab_x + (menu_x / 2) + 2, sets->menu.y - 1, sets->menu.console ? cconsole - 65 : cmain - 65);

		cur_x = 5;
		cur_y = 40;
		cont = tab_max;
	}

	void create_tabs()
	{
		for (int tab_id = legit; tab_id < tab_max; tab_id++)
		{
			color tab = sets->menu.cur_tab == tab_id ? color::opened_tab() : color::closed_tab();
			color text = sets->menu.cur_tab == tab_id ? color::ptext() : color::text();
			/*int x = sets->menu.x + ((170 / (tab_max)) * tab_id);
			int x1 = sets->menu.x + ((170 / (tab_max)) * (tab_id + 1));
			x1 = min(x1, sets->menu.x + 250);
			int y = sets->menu.y + 30;*/

			float factor = (float)((float)menu_x / (float)tab_max);
			int x = (float)sets->menu.x + (float)(factor * (float)tab_id);
			int x1 = x + factor;
			int y = sets->menu.y + 30;
			int tx = x + (factor / 2.f);

			if ((sets->menu.cont_hovered == -1 || sets->menu.cont_hovered == tab_id) && in_range(x, sets->menu.y, global::mouse.x, global::mouse.y, x1, y))
			{
				tab += 20;
				text += 20;

				if (global::key[VK_LBUTTON])
				{
					tab += 20;
					text += 20;
					if (sets->menu.cur_tab != tab_id)
					{
						sets->menu.cur_tab = sets->menu.cont_hovered = tab_id;
					}
				}
				else
				{
					sets->menu.cont_hovered = -1;
				}
			}
			else if (sets->menu.cont_hovered == tab_id)
			{
				sets->menu.cont_hovered = -1;
			}

			prim::filled_box(x, sets->menu.y, x1, y, tab); // основа
			font::draw(font::tab, tx, sets->menu.y + 15, text, DT_CENTER | DT_VCENTER, name[tab_id]);
		}
	}

	void checkbox(int tab, const char* name, bool& value)
	{
		if (sets->menu.cur_tab != tab)
			return;

		cont++;

		if ((cur_y + 9) >= (menu_y - 5))
		{
			cur_x = menu_x / 2;
			cur_y = 40;
		}

		color box = value ? color::enabled() : color::disabled();
		color text = value ? color::ptext() : color::text();

		int x = sets->menu.x + cur_x;
		int y = sets->menu.y + cur_y;

		rect size = font::size(font::cont, name);

		if ((sets->menu.cont_hovered == -1 || sets->menu.cont_hovered == cont) && (
			in_range(x, y, global::mouse.x, global::mouse.y, x + 8, y + 8) ||
			in_range(x + 12, y + 3 - (size.top / 2), global::mouse.x, global::mouse.y, x + 12 + size.left, y + 2 + (size.top / 2))))
		{
			box += 20;
			text += 20;

			if (global::key_click[VK_LBUTTON])
			{
				box += 20;
				text += 20;
				value = !value;
				sets->menu.cont_hovered = cont;
			}
		}
		else if (sets->menu.cont_hovered == cont)
		{
			sets->menu.cont_hovered = -1;
		}

		prim::bordered_box(x - 1, y - 1, x + 9, y + 9, color::outline());
		prim::filled_box(x, y, x + 8, y + 8, box);
		font::draw(font::cont, x + 12, y + 3, text, DT_VCENTER, name);

		cur_y += 14;
	}

	//TODO : сделать +- кнопки по бокам
	template< typename T >
	void slider(int tab, const char* name, T& value, T pmin, T pmax, const char* print_type = "%i")
	{
		if (sets->menu.cur_tab != tab)
			return;

		cont++;

		if ((cur_y + 16) >= (menu_y - 5))
		{
			cur_x = menu_x / 2;
			cur_y = 40;
		}

		color filled = color::enabled();
		color text = color::text();

		int x = sets->menu.x + cur_x;
		int x1 = x + 90;
		int y = sets->menu.y + cur_y;
		int y1 = y + 15;

		if ((sets->menu.cont_hovered == -1 || sets->menu.cont_hovered == cont) && in_range(x, y, global::mouse.x, global::mouse.y, x1, y1))
		{
			filled += 10;
			text += 30;

			if (global::key[VK_LBUTTON])
			{
				filled += 7;
				sets->menu.cont_hovered = cont;
			}
		}

		if (sets->menu.cont_hovered == cont)
		{
			if (global::key[VK_LBUTTON])
			{
				T slider_value = (T)((T)global::mouse.x - (T)x);
				value = pmin + (((float)slider_value / 90.f) * (pmax - pmin));
			}
			else sets->menu.cont_hovered = -1;
		}

		auto size = font::size(font::cont, print_type, value);

		value = max(min(value, pmax), pmin);
		int fill_pos = (float)((float)(value - pmin) / fabs(pmax - pmin)) * 90.f;
		int x_text = max(x + (size.right / 2), x + (fill_pos / 2));

		prim::bordered_box(x - 1, y - 1, x1 + 1, y1 + 1, color::outline());
		prim::filled_box(x, y, x1, y1, color::disabled());
		prim::filled_box(x, y, x + fill_pos, y1, filled);
		font::draw(font::cont, x1 + 5, y + 7, text, DT_VCENTER, name);
		font::draw(font::cont, x_text, y + 7, text + 40, DT_CENTER | DT_VCENTER, print_type, value);

		cur_y += 22;
	}

	/*void switcher(int tab, const char* name, string* value_name, int& value, int size)
	{
		if (sets->menu.cur_tab != tab)
			return;

		cont++;

		color p_color = (value_name[value] == "off" || value_name[value] == "none") ? color::text() : color::ptext();

		string p_value_name = str("< " + value_name[value] + " >");
		
		rect tsize = font::size(font::cont, p_value_name.c_str());
		rect t2size = font::size(font::cont, name);

		bool reverse = t2size.right > tsize.right;

		int x = sets->menu.x + cur_x;
		int y = sets->menu.y + cur_y;
		int dx = x + (reverse ? (t2size.right / 2) : (tsize.right / 2));
		int dy = y + tsize.bottom + 2;

		if (in_range(reverse ? dx : x, y, global::mouse.x, global::mouse.y, (reverse ? dx : x) + tsize.right, y + tsize.bottom))
		{
			p_color += 15;

			if (global::key[VK_LBUTTON])
			{
				sets->menu.cont_hovered = cont;

				if (global::key_click[VK_LBUTTON])
				{
					p_color += 15;

					if (global::mouse.x > dx)
					{
						value++;

						if (value > size)
						{
							value = 0;
						}
					}
					else
					{
						value--;

						if (value < 0)
						{
							value = size;
						}
					}
				}
			}
		}

		font::draw(font::cont, reverse ? dx : x, y, p_color, reverse ? NULL : DT_CENTER, p_value_name.c_str());
		font::draw(font::cont, reverse ? x : dx, dy, p_color - 12, reverse ? NULL : DT_CENTER, name);

		cur_y += (tsize.bottom * 2) + 4;

		if (cur_y >= 275)
		{
			cur_x = 155;
			cur_y = 40;
		}
	}*/

	class combo_t
	{
	public:
		combo_t(int tab, const char* name, const char** names, int* value, int size)
		{
			this->selectable = false;

			this->tab = tab;
			this->name = name;
			this->names = names;
			this->value = value;
			this->size = size;

			this->on_logic();
		}

		combo_t(int tab, const char* name, const char** names, bool* values, int size)
		{
			this->selectable = true;

			this->tab = tab;
			this->name = name;
			this->names = names;
			this->values = values;
			this->size = size;

			this->on_logic();
		}

		void on_logic()
		{
			if (sets->menu.cur_tab != tab)
				return;

			cont++;

			if ((cur_y + 16) >= (menu_y - 5))
			{
				cur_x = menu_x / 2;
				cur_y = 40;
			}

			this->p_cont = cont;

			this->x = sets->menu.x + cur_x;
			this->x1 = x + 90;
			this->y = sets->menu.y + cur_y;
			this->y1 = y + 15;

			cur_y += 22;
		}

		void on_draw()
		{
			if (sets->menu.cur_tab != tab)
				return;

			color background = color::disabled();
			color text = color::text() + 40;

			bool key_click = global::key_click[VK_LBUTTON];

			str selected = "";

			if (sets->menu.cont_hovered == -1 && in_range(x, y, global::mouse.x, global::mouse.y, x1, y1))
			{
				background += 10;
				text += 20;

				if (key_click)
				{
					background += 5;
					text += 10;
					sets->menu.cont_hovered = p_cont;
					//opened = true;
					key_click = false;
				}
			}

			for (int value_id = 0; value_id < size; value_id++)
			{
				if (sets->menu.cont_hovered != p_cont)
				{
					if (selectable && values[value_id])
					{
						if (selected.empty())
						{
							selected += names[value_id];
						}
						else if (!strstr(selected.c_str(), "etc"))
						{
							auto ssize = font::size(font::cont, (selected + names[value_id] + ", etc").c_str());

							selected += ", ";
							if (ssize.right <= 85) selected += names[value_id];
							else selected += "etc";
						}
					}
					else if (!selectable) break;

					continue;
				}

				color vbackground = color::disabled();
				color vtext = color::text() + 40;

				int vy = y + (15 * value_id);
				int vy1 = y + (15 * (value_id + 1)) - 1;

				if ((!selectable && value_id == *value) || (selectable && values[value_id]))
					vtext = color::enabled() + 65; // цвет странно работает

				if (in_range(x, vy, global::mouse.x, global::mouse.y, x1, vy1))
				{
					vbackground += 15;
					vtext += 25;

					if (key_click)
					{
						vbackground += 7;
						vtext += 14;

						if (selectable)
						{
							values[value_id] = !values[value_id];
						}
						else
						{
							*value = value_id;

							sets->menu.cont_hovered = -1;
						}
						//opened = false;
					}
				}
				//else if (value_id == (size - 1) && key_click) sets->menu.cont_hovered = -1;

				prim::filled_box(x, vy, x1, vy1, vbackground);
				prim::bordered_box(x - 1, vy - 1, x1 + 1, vy1 + 1, color::outline());
				font::draw(font::cont, x + 45, vy + 7, vtext, DT_CENTER | DT_VCENTER, names[value_id]);
			}

			if (sets->menu.cont_hovered != p_cont)
			{
				//if (!global::key[VK_LBUTTON] && sets->menu.cont_hovered == p_cont)
				//	sets->menu.cont_hovered = -1;

				if (selected.empty()) selected = "nothing";
				prim::filled_box(x, y, x1, y1, background);
				prim::bordered_box(x - 1, y - 1, x1 + 1, y1 + 1, color::outline());
				font::draw(font::cont, x + 45, y + 7, text, DT_CENTER | DT_VCENTER, selectable ? selected.c_str() : names[*value]);
			}
			else if (!in_range(x, y, global::mouse.x, global::mouse.y, x + 90, y + (15 * ((size - 1) + 1))) && key_click)
			{
				sets->menu.cont_hovered = -1;
			}

			font::draw(font::cont, x1 + 5, y + 7, color::text(), DT_VCENTER, name);
		}

	private:
		int tab, size, *value, x, x1, y, y1, p_cont;
		const char* name, **names;
		bool selectable, *values;
	};

	/*void combo(int tab, const char* name, const char** names, int& value, int size)
	{
		if (sets->menu.cur_tab != tab)
			return;

		cont++;

		color background = color::disabled();
		color text = color::text() + 40;

		int x = sets->menu.x + cur_x;
		int x1 = x + 90;
		int y = sets->menu.y + cur_y;
		int y1 = y + 15;

		bool key_click = global::key_click[VK_LBUTTON];

		if (sets->menu.cont_hovered == -1 && in_range(x, y, global::mouse.x, global::mouse.y, x1, y1))
		{
			background += 10;
			text += 20;

			if (key_click)
			{
				background += 5;
				text += 10;
				sets->menu.cont_hovered = cont;
				key_click = false;
			}
		}

		if (sets->menu.cont_hovered == cont)
		{
			for (int value_id = 0; value_id < size; value_id++)
			{
				color vbackground = color::disabled();
				color vtext = color::text() + 40;

				int vy = y + (15 * value_id);
				int vy1 = y + (15 * (value_id + 1));

				if (value_id == value)
					vtext = color::enabled();

				if (in_range(x, vy, global::mouse.x, global::mouse.y, x1, vy1))
				{
					vbackground += 15;
					vtext += 25;

					if (key_click)
					{
						vbackground += 7;
						vtext += 14;

						value = value_id;
						sets->menu.cont_hovered = -1;
					}
				}

				prim::filled_box(x, vy, x1, vy1, vbackground);
				prim::bordered_box(x - 1, vy - 1, x1 + 1, vy1 + 1, color::outline());
				font::draw(font::cont, x + 45, vy + 7, vtext, DT_CENTER | DT_VCENTER, names[value_id]);
			}
		}
		else
		{
			prim::filled_box(x, y, x1, y1, background);
			prim::bordered_box(x - 1, y - 1, x1 + 1, y1 + 1, color::outline());
			font::draw(font::cont, x + 45, y + 7, text, DT_CENTER | DT_VCENTER, names[value]);
		}

		font::draw(font::cont, x + 95, y + 7, color::text(), DT_VCENTER, name);

		cur_y += 19;

		if (cur_y >= 275)
		{
			cur_x = 155;
			cur_y = 40;
		}
	}*/

	bool button(int tab, const char* name, int p_x, int p_y, int width, int height)
	{
		if (tab != -1 && tab != sets->menu.cur_tab) return false;

		cont++;

		color background = color::disabled();
		color text = color::ptext();

		int x = sets->menu.x + p_x;
		int x1 = x + width;
		int y = sets->menu.y + p_y;
		int y1 = y + height;

		bool value = false;

		if (sets->menu.cont_hovered == -1 && in_range(x, y, global::mouse.x, global::mouse.y, x1, y1))
		{
			background += 10;
			text += 20;

			if (global::key_click[VK_LBUTTON] && sets->menu.cont_hovered == -1)
			{
				background += 5;
				text += 10;
				value = true;
				sets->menu.cont_hovered = cont;
			}
		}

		if (!value && sets->menu.cont_hovered == cont)
		{
			sets->menu.cont_hovered = -1;
		}

		prim::bordered_box(x - 1, y - 1, x1 + 1, y1 + 1, color::outline());
		prim::filled_box(x, y, x1, y1, background);
		font::draw(font::cont, x + (width / 2), y + (height / 2), text, DT_CENTER | DT_VCENTER, name);

		return value;
	}

	void keybinder(int tab, const char* name, bind_t& value)
	{
		if (sets->menu.cur_tab != tab)
			return;

		checkbox(tab, str(name + str(".hold")).c_str(), value.hold);

		cont++;

		if ((cur_y + 16) >= (menu_y - 5))
		{
			cur_x = menu_x / 2;
			cur_y = 40;
		}

		color background = color::disabled();
		color selected = color::text() + 40;

		int x = sets->menu.x + cur_x;
		int x1 = x + 90;
		int y = sets->menu.y + cur_y;
		int y1 = y + 15;

		bool key_click = global::key_click[VK_LBUTTON];

		const char* keyname = "";

		if (sets->menu.cont_hovered == cont) keyname = "press any key";
		else if (value.key > 0) keyname = keynames[value.key];
		else keyname = "no bind";

		if ((sets->menu.cont_hovered == -1 || sets->menu.cont_hovered == cont) && in_range(x, y, global::mouse.x, global::mouse.y, x1, y1))
		{
			background += 10;
			selected += 20;

			if (key_click)
			{
				background += 10;
				selected += 20;
				value.key = 0;
				sets->menu.cont_hovered = cont;
				key_click = false;
			}
		}

		if (sets->menu.cont_hovered == cont)
		{
			selected = color::enabled() + 65;

			if (key_click)
			{
				value.key = VK_LBUTTON;
				sets->menu.cont_hovered = -1;
			}
			else
			{
				for (int key_id = 2; key_id < 254; key_id++)
				{
					if (global::key_click[key_id] && keynames[key_id] != null)
					{
						if (key_id == VK_ESCAPE) value.key = 0;
						else value.key = key_id;
						sets->menu.cont_hovered = -1;
					}
				}
			}
		}

		prim::bordered_box(x - 1, y - 1, x1 + 1, y1 + 1, color::outline());
		prim::filled_box(x, y, x1, y1, background);
		font::draw(font::cont, x + 45, y + 7, selected, DT_CENTER | DT_VCENTER, value.key > 0 ? keynames[value.key] : "no bind");
		font::draw(font::cont, x1 + 5, y + 7, color::text(), DT_VCENTER, name);

		cur_y += 22;
	}

	int pageswitcher(int tab, const char** names, int size, int p_x, int p_y)
	{
		if (tab == -1 || tab != sets->menu.cur_tab) return -1;

		color color = color::text();

		int x = sets->menu.x + p_x;
		int y = sets->menu.y + p_y;

		if (button(tab, "<", p_x, p_y, 14, 14))
		{
			page[tab]--;
			if (page[tab] < 0) page[tab] = 0;
			color += 20;
		}
		if (button(tab, ">", p_x + 60, p_y, 14, 14))
		{
			page[tab]++;
			if (page[tab] >= size) page[tab] = size - 1;
			color += 20;
		}
		
		font::draw(font::cont, x - 3, y + 7, color, DT_VCENTER | DT_RIGHT, str(LM_VER + " " + str(names[page[tab]])).c_str());
		font::draw(font::cont, x + 37, y + 7, color, DT_VCENTER | DT_CENTER, "%i / %i", page[tab] + 1, size);

		return page[tab];
	}

	void rgb(int tab, const char* name, color& value)
	{
		if (sets->menu.cur_tab != tab)
			return;

		slider<int>(tab, str(name + str(".r")).c_str(), value.r, 0, 255);
		slider<int>(tab, str(name + str(".g")).c_str(), value.g, 0, 255);
		slider<int>(tab, str(name + str(".b")).c_str(), value.b, 0, 255);

		cont++;

		if ((cur_y + 16) >= (menu_y - 5))
		{
			cur_x = menu_x / 2;
			cur_y = 40;
		}

		int x = sets->menu.x + cur_x;
		int x1 = x + 90;
		int y = sets->menu.y + cur_y;
		int y1 = y + 15;

		prim::bordered_box(x - 1, y - 1, x1 + 1, y1 + 1, color::outline());
		prim::filled_box(x, y, x1, y1, value);
		font::draw(font::cont, x1 + 5, y + 7, color::text(), DT_VCENTER, name);

		cur_y += 22;
	}
		
	//TODO : пофиксить отрисовку линий и тд и сделать нормальный курсор
	void draw_mouse()
	{
		prim::filled_box(global::mouse.x - 3, global::mouse.y - 3, global::mouse.x + 3, global::mouse.y + 3, color::lm());
	}
}