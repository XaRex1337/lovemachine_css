#pragma once
#include "obj'ects.h"
#include "obj_console.h"
#include "obj_information.h"
#include "obj_spectators.h"
#include "backtrack.h"

namespace menu
{
	void draw()
	{
		if (global::key_click[VK_INSERT])
		{
			sets->menu.opened = !sets->menu.opened;
			//console::write(sets->menu.opened ? "menu opened" : "menu closed", sets->menu.opened ? color(0, 170, 0) : color(170, 0, 0));
		}

		obj::information::on_draw();
		obj::spectators::on_draw();

		if (sets->menu.opened)
		{
			//*global::lock_cursor = false;
			legit::backtrack::draw = false;

			obj::on_open();

			if (sets->menu.console)
			{
				if (!console::lines.empty())
				{
					if (console::lines.size() > 10)
					{
						console::lines.erase(console::lines.begin());
					}

					for (int line_id = 0; line_id < console::lines.size(); line_id++)
					{
						auto line = console::lines.at(line_id);
						font::draw(font::cont, sets->menu.x + 5, sets->menu.y + (16 * line_id), line.ccolor, null, line.text.c_str());
					}
				}

				static string text;
				obj::con::textgetter("", text, 5, obj::menu_y - 25, obj::menu_x - 100, 20);
				if (obj::button(-1, "send", obj::menu_x - 90, obj::menu_y - 25, 85, 20))
				{
					obj::con::do_analysis(text);
					text = "";
				}
			}
			else
			{
				obj::create_tabs();

				int page = -1;
				page = obj::pageswitcher(sets->menu.cur_tab, sets->menu.cur_tab == obj::legit ? obj::pages_legit : sets->menu.cur_tab == obj::visuals ? obj::pages_visuals : obj::pages_misc, sets->menu.cur_tab == obj::legit ? ARRAYSIZE(obj::pages_legit) : sets->menu.cur_tab == obj::visuals ?  ARRAYSIZE(obj::pages_visuals) : ARRAYSIZE(obj::pages_misc), obj::menu_x - 79, obj::menu_y - 19);
			
				switch (page)
				{
				case 0:
				{
					obj::checkbox(obj::legit, "enabled", sets->legit.enabled);
					obj::checkbox(obj::legit, "friends", sets->legit.friends);
					obj::checkbox(obj::legit, "knifebot", sets->legit.knifebot);
					if (cvar(ragemode).value)
					{
						//obj::checkbox(obj::legit, "autoscope", sets->rage.autoscope);
						obj::checkbox(obj::legit, "autowall", sets->rage.autowall);
						obj::checkbox(obj::legit, "autoshoot", sets->rage.autoshoot);
						obj::checkbox(obj::legit, "autostop", sets->rage.autostop);
						if (!cvar(antismac).value) obj::checkbox(obj::legit, "silent", sets->rage.silent);
					}
					obj::checkbox(obj::legit, "backtrack", sets->legit.backtrack.enabled);
					obj::combo_t bt_bone(obj::legit, "bt_bone", obj::hitbox, sets->legit.backtrack.hitbox, ARRAYSIZE(obj::hitbox));
					obj::slider<int>(obj::legit, "ticks", sets->legit.backtrack.ticks, 1, 14);
					obj::combo_t bt_style(obj::legit, "style", obj::backtrack_style, sets->legit.backtrack.style, ARRAYSIZE(obj::backtrack_style));
					bt_style.on_draw();
					bt_bone.on_draw();

					obj::checkbox(obj::visuals, "enabled", sets->visuals.enabled);
					obj::checkbox(obj::visuals, "friends", sets->visuals.friends);
					//obj::combo(obj::visuals, "chams", obj::chams, sets->visuals.chams, ARRAYSIZE(obj::chams));
					obj::combo_t chams(obj::visuals, "chams", obj::chams, &sets->visuals.chams, ARRAYSIZE(obj::chams));
					obj::combo_t chams_style(obj::visuals, "style", obj::chams_style, sets->visuals.chams_style, ARRAYSIZE(obj::chams_style));
					obj::combo_t other_chams(obj::visuals, "other", obj::other_chams, sets->visuals.other_chams, ARRAYSIZE(obj::other_chams));
					obj::combo_t other_style(obj::visuals, "o_style", obj::other_style, sets->visuals.other_style, ARRAYSIZE(obj::other_style));
					obj::combo_t crosshair(obj::visuals, "crosshair", obj::crosshair, &sets->visuals.crosshair, ARRAYSIZE(obj::crosshair));
					obj::combo_t esp_filter(obj::visuals, "esp_filter", obj::esp_filter, sets->visuals.esp_filter, ARRAYSIZE(obj::esp_filter));
					obj::combo_t esp_show(obj::visuals, "esp_show", obj::esp_show, sets->visuals.esp_show, ARRAYSIZE(obj::esp_show));
					obj::combo_t esp_bar(obj::visuals, "esp_bar", obj::esp_bar, sets->visuals.esp_bar, ARRAYSIZE(obj::esp_bar));
					obj::combo_t esp_check(obj::visuals, "esp_check", obj::esp_check, sets->visuals.esp_check, ARRAYSIZE(obj::esp_check));
					obj::checkbox(obj::visuals, "fade", sets->visuals.fade);
					obj::checkbox(obj::visuals, "defuser_only_if_need", sets->visuals.defuser_only_if_need);
					//obj::checkbox(obj::visuals, "fancy_w2s", sets->visuals.fancy_w2s);
					esp_check.on_draw();
					esp_bar.on_draw();
					esp_show.on_draw();
					esp_filter.on_draw();
					crosshair.on_draw();
					other_style.on_draw();
					other_chams.on_draw();
					chams_style.on_draw();
					chams.on_draw();

					obj::checkbox(obj::misc, "autopistol", sets->misc.autopistol);
					obj::checkbox(obj::misc, "autojump", sets->misc.autojump);
					obj::checkbox(obj::misc, "autostrafer", sets->misc.autostrafer);
					obj::slider<int>(obj::misc, "aj_percent", sets->misc.aj_percent, 0, 100);
					obj::slider<int>(obj::misc, "fake_ping", sets->misc.fake_ping, 0, 1000);
					obj::checkbox(obj::misc, "killshot", sets->misc.killshot);
				}
				break;
				case 1:
				{
					obj::combo_t aim_bone(obj::legit, "aim_bone", obj::hitbox, sets->legit.aim.hitbox, ARRAYSIZE(obj::hitbox));
					obj::slider<float>(obj::legit, "fov", sets->legit.aim.fov, 0.f, cvar(ragemode).value ? 180.f : 10.f, "%.1f");
					if (!cvar(ragemode).value)
					{
						obj::slider<float>(obj::legit, "smooth_x", sets->legit.aim.smooth[0], 0.f, 50.f, "%.1f");
						obj::slider<float>(obj::legit, "smooth_y", sets->legit.aim.smooth[1], 0.f, 50.f, "%.1f");
						obj::slider<float>(obj::legit, "rcs_x", sets->legit.aim.rcs[0], 0.f, 2.f, "%.2f");
						obj::slider<float>(obj::legit, "rcs_y", sets->legit.aim.rcs[1], 0.f, 2.f, "%.2f");
						obj::slider<float>(obj::legit, "humanize_x", sets->legit.aim.humanize[0], 0.f, 2.f, "%.2f");
						obj::slider<float>(obj::legit, "humanize_y", sets->legit.aim.humanize[1], 0.f, 2.f, "%.2f");
						obj::slider<float>(obj::legit, "kill_delay", sets->legit.aim.kill_delay, 0.f, 1.f, "%.2f");
						obj::slider<float>(obj::legit, "shot_delay", sets->legit.aim.shot_delay, 0.f, 0.2f, "%.3f");
					}
					else
					{
						obj::slider<float>(obj::legit, "hitchance", sets->rage.hitchance, 0.f, 1.f, "%f");
					}
					aim_bone.on_draw();

					//obj::checkbox(obj::visuals, "info_box", sets->info.opened);
					//obj::checkbox(obj::visuals, "spectators", sets->spec.opened);
					//obj::combo_t info_style(obj::visuals, "info_style", obj::info_style, sets->info.style, ARRAYSIZE(obj::info_style));
					//obj::combo_t ak47_skin(obj::visuals, "ak47_skin", obj::ak47_skin, &sets->visuals.ak47_skin, ARRAYSIZE(obj::ak47_skin));
					//obj::combo_t deagle_skin(obj::visuals, "deagle_skin", obj::deagle_skin, &sets->visuals.deagle_skin, ARRAYSIZE(obj::deagle_skin));
					//obj::combo_t knife_skin(obj::visuals, "knife_skin", obj::knife_skin, &sets->visuals.knife_skin, ARRAYSIZE(obj::knife_skin));
					//obj::combo_t other_skins(obj::visuals, "other_skins", obj::other_skins, sets->visuals.other_skins, ARRAYSIZE(obj::other_skins));
					obj::combo_t remove(obj::visuals, "remove", obj::remove, sets->visuals.remove, ARRAYSIZE(obj::remove));
					obj::checkbox(obj::visuals, "hitmarker", sets->visuals.hitmarker);
					obj::checkbox(obj::visuals, "bomb_timer", sets->visuals.bomb_timer);
					remove.on_draw();
					//other_skins.on_draw();
					//knife_skin.on_draw();
					//deagle_skin.on_draw();
					//ak47_skin.on_draw();
					//info_style.on_draw();

					obj::combo_t lag_mode(obj::misc, "lag_mode", obj::lag_mode, &sets->misc.lag_mode, ARRAYSIZE(obj::lag_mode));
					obj::keybinder(obj::misc, "lag_spam", sets->misc.lag_spam);
					obj::slider<int>(obj::misc, "lag_factor", sets->misc.lag_factor, 0, 14);
					obj::checkbox(obj::misc, "fl_spam_always", sets->misc.fl_spam_always);
					obj::keybinder(obj::misc, "fl_spam", sets->misc.fl_spam);
					obj::keybinder(obj::misc, "airstuck", sets->misc.airstuck);
					obj::keybinder(obj::misc, "slowmotion", sets->misc.slowmotion);
					obj::slider<int>(obj::misc, "sm_speed", sets->misc.sm_speed, 2, 5);
					//obj::keybinder(obj::misc, "record", sets->misc.record);
					//obj::keybinder(obj::misc, "play", sets->misc.play);
					lag_mode.on_draw();
				}
				break;
				case 2:
				{
					obj::checkbox(obj::legit, "enabled", sets->legit.trigger._enabled);
					obj::keybinder(obj::legit, "bind", sets->legit.trigger.bind);
					obj::combo_t hitbox(obj::legit, "hitbox", obj::hitbox, sets->legit.trigger.hitbox, ARRAYSIZE(obj::hitbox));
					obj::slider<float>(obj::legit, "delay", sets->legit.trigger.delay, 0.f, 0.1f, "%.3f");
					hitbox.on_draw();

					obj::rgb(obj::misc, "chams_t", sets->visuals.chams_t);
					if (sets->menu.cur_tab == obj::misc) obj::cur_y += 100;
					obj::rgb(obj::misc, "chams_ct", sets->visuals.chams_ct);
				}
				break;
				case 3:
				{
					obj::rgb(obj::misc, "esp_t", sets->visuals.esp_t);
					if (sets->menu.cur_tab == obj::misc) obj::cur_y += 100;
					obj::rgb(obj::misc, "esp_ct", sets->visuals.esp_ct);
				}
				break;
				case 4:
					obj::combo_t draw_mode(obj::misc, "draw_mode", obj::draw_mode, &sets->misc.draw_mode, ARRAYSIZE(obj::draw_mode));
					obj::slider<float>(obj::misc, "draw_time", sets->misc.draw_time, 0.1f, 10.0f, "%.1f");
					obj::keybinder(obj::misc, "draw_start", sets->misc.draw_start);
					obj::keybinder(obj::misc, "draw_clear", sets->misc.draw_clear);
					if (sets->menu.cur_tab == obj::misc) obj::cur_y += 100;
					obj::rgb(obj::misc, "draw_color", sets->misc.draw_color);
					draw_mode.on_draw();

				break;
				}				
			}

			obj::draw_mouse();
		}
		else
		{
			sets->menu.cont_hovered = -1;
		}
	}
}