#pragma once
#include "includes.h"
#include "definitions.h"
#include "game shit.h"
#include "settings.h"
#include "draw.h"
#include "esp.h"

namespace events
{
	using event_callback = void(*)(igameevent*);
	class custom_listener : public igameeventlistener
	{
	public:
		custom_listener(event_callback callback)
		{
			this->callback = callback;
		}
		
		virtual void fire_game_event(igameevent* event)
		{
			callback(event);
		}

		event_callback callback;
	};

	namespace hitmarker
	{
		int damage_did = 0;
		int health_left = 0;
		int killstreak = 0;
		int killstreak2 = 0;
		str text = "KILLED";
		float timer = 0.f;
		float kill_timer = 0.f;

		void draw_box(int pos_x, int pos_y, string text, int alpha)
		{
			//auto size = font::size(font::hitmarker_small, text.c_str());
			//int x = pos_x - (size.right / 2);
			//int y = pos_y - (size.bottom / 2);
			//int x1 = pos_x + (size.right / 2);
			//int y1 = pos_y + (size.bottom / 2);

			//prim::filled_box(x - 4, y - 4, x1 + 4, y1 + 4, color(18, 18, 18, alpha));
			//prim::bordered_box(x - 3, y - 3, x1 + 3, y1 + 3, color(9, 9, 9, alpha));
			//prim::bordered_box(x - 5, y - 5, x1 + 5, y1 + 5, color(9, 9, 9, alpha));
			font::draw(font::hitmarker_small, pos_x, pos_y, color(20, 255, 20, alpha), DT_CENTER | DT_VCENTER, text.c_str());
		}

		void on_fire_event(igameevent* event, const char* name)
		{
			if (strcmp(name, "player_hurt") != 0)
				return;

			health_left = event->get_int("health");
			if (_engine->get_player_for_userid(event->get_int("userid")) == global::local_id && health_left == 0)
			{
				killstreak = 0;
			}

			if (_engine->get_player_for_userid(event->get_int("attacker")) != global::local_id)
				return;

			if (sets->misc.killshot && health_left == 0)
			{
				killstreak2++;
				_engine->clientcmd_unrestricted(str(u8"say Обоссал и выставил на мороз! Streak Of " + to_str(killstreak2) + u8"!").c_str());
			}

			if (sets->visuals.enabled && sets->visuals.hitmarker)
			{
				damage_did += event->get_int("dmg_health");
			}

			if (health_left == 0)
			{
				if (sets->visuals.enabled && sets->visuals.hitmarker)
				{
					killstreak++;
					auto weapon = event->get_string("weapon");
					if (strstr(weapon, "knife")) text = "KNIFED";
					else if (event->get_int("hitgroup") == 1) text = "HEADSHOT";
					else if (strstr(weapon, "grenade") || strstr(weapon, "flash")) text = "GRENADE";
					else text = "KILLED";
					if (killstreak > 1) text += " X" + killstreak;
					kill_timer = global::realtime + 3.f;
				}
				legit::aimbot::kill_delay = sets->legit.aim.kill_delay != 0.f ? global::curtime + sets->legit.aim.kill_delay : 0.f;
			}
			timer = global::realtime + 4.f;
		}
		
		void on_draw()
		{
			if (!sets->misc.killshot)
			{
				killstreak = 0;
			}

			if (!sets->visuals.hitmarker)
			{
				damage_did = 0;
				health_left = 0;
				killstreak = 0;
				timer = 0.f;
				kill_timer = 0.f;
				return;
			}

			if (timer == 0.f && kill_timer == 0.f) return;

			int centerx = global::screen.right / 2;
			int centery = global::screen.bottom / 2;

			if ((timer - global::realtime + 1.f) > 0.f)
			{
				float percent = timer - global::realtime + 1.f;
				int alpha = 70 * percent;
				string s_did = "did " + to_string(damage_did) + " hp";
				string s_left = "left " + to_string(health_left) + " hp";
				draw_box(centerx - 150, centery, s_did, alpha);
				draw_box(centerx + 150, centery, s_left, alpha);
			}
			else
			{
				damage_did = 0;
				health_left = 0;
				killstreak = 0;
				timer = 0.f;
			}

			if ((kill_timer - global::realtime + 0.1f) > 0.f)
			{
				float percent = kill_timer - global::realtime + 1.f;
				int alpha = 127 * percent;
				int y = (centery / 10 * percent) - (centery / 4);
				font::draw(font::hitmarker_big, centerx, y, color(255, 20, 20, alpha), DT_CENTER | DT_VCENTER, text.c_str());
			}
			else
			{
				kill_timer = 0.f;
			}
		}
	}

	namespace bomb_timer
	{
		bool planted = false;
		float explosion_time = 0.f;
		float defuse_time = 0.f;
		float f_exp_time = 0.f;
		float f_def_time = 0.f;

		inline void on_fire_event(igameevent* event, const char* name)
		{
			if (strcmp(name, "bomb_planted") == 0) {
				planted = true;
				explosion_time = 0.f;
				defuse_time = 0.f;
			} else if (strcmp(name, "bomb_exploded") == 0 || strcmp(name, "bomb_defused") == 0) { // end
				planted = false;
				explosion_time = 0.f;
				defuse_time = 0.f;
			} else if (strcmp(name, "bomb_begindefuse") == 0) {
				f_def_time = event->get_bool("haskit") ? 5.f : 10.f;
				defuse_time = global::curtime + f_def_time;
			} else if (strcmp(name, "bomb_abortdefuse") == 0) {
				defuse_time = 0.f;
			}
		}

		void on_draw()
		{
			if (!sets->visuals.enabled || !sets->visuals.bomb_timer || explosion_time == 0.f || !planted) return;

			float percent = ((explosion_time - global::curtime) / f_exp_time);
			int x = global::screen.right * percent;

			prim::filled_box(0, 0, x, 20, color(255 - (255 * percent), 255 * percent, 0));
			font::draw(font::hitmarker_small, x + 5, 20, color(255 * percent, 255 - (255 * percent), 0), DT_CENTER, explosion_time > 0.f ? "%.1f" : "exploded", (explosion_time - global::curtime));
			
			if (defuse_time == 0.f) return;

			float percent2 = ((defuse_time - global::curtime) / f_def_time);
			percent = ((defuse_time - global::curtime) / f_exp_time);
			x = global::screen.right * percent;

			prim::filled_box(0, 0, x, 20, color(0, 255 - (255 * percent2), 255 * percent2));
			font::draw(font::hitmarker_small, x + 5, 10, color(0, 255 * percent2, 255 - (255 * percent2)), DT_CENTER | DT_VCENTER, "%.1f", (defuse_time - global::curtime));
		}
	}

	inline void on_fire_event(igameevent* event, const char* name)
	{
		if (strcmp(name, "round_start") == 0)
		{
			hitmarker::killstreak2 = hitmarker::timer = hitmarker::kill_timer = bomb_timer::defuse_time = bomb_timer::explosion_time = legit::aimbot::kill_delay = 0.f;
			bomb_timer::planted = false;
			misc::draw::clear(false);
			//esp::sounds.clear(); // TODO: пофиксить, не компилится
			//ZeroMemory(legit::backtrack::records, sizeof(legit::backtrack::records));
			//ZeroMemory(server::players, sizeof(server::players));
		}

		hitmarker::on_fire_event(event, name);
		bomb_timer::on_fire_event(event, name);
	}

	void on_draw()
	{
		hitmarker::on_draw();
		bomb_timer::on_draw();
	}
}