#pragma once
#include "game shit.h"
#include "game def's.h"
#include "game classes.h"
#include "events.h"
#include "settings.h"
#include "surface.h"

namespace esp
{
	struct cbox
	{
		cbox() {}

		cbox(/*int id, */centity* entity, int class_id)
		{
			const auto collideable = entity->get_collideable();
			if (!collideable) return;

			/*if (sets->visuals.fancy_w2s)
			{
				this->mins = collideable->obb_mins();
				this->maxs = collideable->obb_maxs();
				this->coord_frame = collideable->collision_to_world_transform();
				this->origin = entity->get_abs_origin();
				return;
			}*/

			//this->maxs = collideable->obb_maxs();
			//this->maxs.z += 10.f;
			//if (sets->visuals.fancy_w2s && server::entity[id].valid) this->coord_frame = server::entity[id].coord_frame;
			if (class_id == CCSPlayer)
			{
				//if (sets->visuals.fancy_w2s) this->mins = collideable->obb_mins();
				//else
				//{
					this->origin = entity->get_abs_origin();
					this->max_origin = this->origin + Vector(0, 0, collideable->obb_maxs().z);
					this->origin.z -= 10.f;
					this->max_origin.z += 10.f;
				//}
			}
			else
			{
				//this->mins = collideable->obb_mins();
				//if (sets->visuals.fancy_w2s)
				//{
					//this->origin = entity->get_abs_origin();
				//}
				//else
				//{
					this->origin = entity->get_abs_origin();
					this->max_origin = this->origin + collideable->obb_maxs();
					this->origin += collideable->obb_mins();
				//}
			}

			//this->id = id;
			this->class_id = class_id;
		}

		bool construct_points()
		{
			cvector screen, max_screen;

			/*if (sets->visuals.fancy_w2s)
			{
				cvector points[8] =
				{
					this->mins, this->maxs,
					cvector(mins.x, maxs.y, maxs.z),
					cvector(mins.x, mins.y, maxs.z),
					cvector(maxs.x, mins.y, mins.z),
					cvector(maxs.x, maxs.y, mins.z),
					cvector(mins.x, maxs.y, mins.z),
					cvector(maxs.x, mins.y, maxs.z)
				};

				cvector p_point;
				for (auto point : points)
				{
					//p_point = this->origin + point;

					vector_transform(p_point, this->coord_frame, p_point);

					if (!w2s(p_point, screen))
						return false;

					//if (!obj::in_range(0, 0, screen.x, screen.y, global::screen.right, global::screen.bottom))
					//	continue;

					if (screen.x < left) left = screen.x;
					if (screen.x > right) right = screen.x;
					if (screen.y < top) top = screen.y;
					if (screen.y > bottom) bottom = screen.y;
				}

				centerx = left + ((right - left) / 2);
				return false;
			}*/

			if (class_id == CCSPlayer/* && !sets->visuals.fancy_w2s*/)
			{
				if (!w2s(origin, screen) || !w2s(max_origin, max_screen))
					return false;

				height = screen.y - max_screen.y;
				width = height / 4;

				left = screen.x - width;
				right = screen.x + width;
				top = max_screen.y;
				bottom = screen.y;
				centerx = screen.x;
			}
			else// if (!sets->visuals.fancy_w2s)
			{
				if (!w2s(origin, screen) || !w2s(max_origin, max_screen))
					return false;

				left = min(screen.x, max_screen.x) - 5;
				right = max(screen.x, max_screen.x) + 5;
				top = min(screen.y, max_screen.y) - 5;
				bottom = max(screen.y, max_screen.y) + 5;

				width = right - left;
				height = bottom - top;

				centerx = left + (width / 2);
			}
			/*else if (server::entity[id].valid)
			{
				cvector points[8] =
				{
					this->mins, this->maxs,
					cvector(mins.x, maxs.y, maxs.z),
					cvector(mins.x, mins.y, maxs.z),
					cvector(maxs.x, mins.y, mins.z),
					cvector(maxs.x, maxs.y, mins.z),
					cvector(mins.x, maxs.y, mins.z),
					cvector(maxs.x, mins.y, maxs.z)
				};

				cvector p_point;
				for (auto point : points)
				{
					//p_point = this->origin + point;

					vector_transform(p_point, server::entity[id].coord_frame, p_point);

					if (!w2s(p_point, screen))
						return false;

					//if (!obj::in_range(0, 0, screen.x, screen.y, global::screen.right, global::screen.bottom))
					//	continue;

					if (screen.x < left) left = screen.x;
					if (screen.x > right) right = screen.x;
					if (screen.y < top) top = screen.y;
					if (screen.y > bottom) bottom = screen.y;
				}

				centerx = left + ((right - left) / 2);
			}
			else return false;*/

			return true;
		}

		bool visible(centity* entity, /*int class_id,*/ matrix3x4_t matrix[128] = nullptr)
		{
			if (!is_hitbox_visible(entity, hitbox_head, matrix)/*entity->hitbox_visible(hitbox_head, matrix)*/ && 
				!is_hitbox_visible(entity, hitbox_stomach, matrix)/*entity->hitbox_visible(hitbox_stomach, matrix)*/)
				return false;
			//else if (class_id != CCSPlayer && (!entity->visible(origin) && !entity->visible(max_origin)))
			//	return false;

			return true;
		}

		matrix3x4_t coord_frame;
		cvector mins, maxs, origin, max_origin;
		int /*id,*/ class_id, width, height, left, right, top, bottom, centerx;
	};

	void circle_3d(cvector pos, float radius, float resolution, color col)
	{
		// cos - x, sin - y
		const float j = M_PI * 2.f / resolution;
		for (float i = 0.f; i < M_PI * 2.f; i += j)
		{
			float x1, y1, x2, y2;
			sincos(i, &y1, &x1);
			sincos(i + j, &y2, &x2);

			cvector screen1, screen2;
			if (!w2s(cvector(pos.x + x1 * radius, pos.y + y1 * radius, pos.z), screen1))
				continue;

			if (!w2s(cvector(pos.x + x2 * radius, pos.y + y2 * radius, pos.z), screen2))
				continue;

			surf::prim::line(screen1.x, screen1.y, screen2.x, screen2.y, col);
			//surf::prim::line(screen1.x + 1, screen1.y + 1, screen2.x + 1, screen2.y + 1, col);
		}
	}

	void draw_sounds()
	{
		if (!server::sounds.empty())
		{
			for (int i = 0; i < server::sounds.size(); i++)
			{
				if (abs(server::sounds.at(i).time - global::curtime) > 2.f)
				{
					server::sounds.erase(server::sounds.begin() + i);
					i--;
					continue;
				}
				// TODO: сделать так, чтобы линия становилась кругом, удлиняясь, а после укорачивалась и исчезала
				circle_3d(server::sounds.at(i).position,
					5.f + abs(server::sounds.at(i).time - global::curtime) * 5.f, 14.f,
					server::sounds.at(i).col.with_alpha(255.f - abs(server::sounds.at(i).time - global::curtime) * 127.5f));
			}
		}
	}

	template<typename t>
	void bar(bool vertical, bool text_on_left, int left, int top, int right, int bottom, float value, float vmax, color front, color back, const char* output = "%i")
	{
		surf::prim::filled_box(left - 1, top - 1, right + 1, bottom + 1, color::outline().with_alpha(front.a));
		surf::prim::filled_box(left, top, right, bottom, back);

		const auto amount = vertical ? max(top, bottom - ((bottom - top) * (float)(value / vmax))) : min(right, left + ((right - left) * (float)(value / vmax)));

		surf::prim::filled_box(left, vertical ? amount : top, vertical ? right : amount, bottom, front);
		if (value != vmax)
		{
			//surf::prim::filled_box(vertical ? left : amount - 1, vertical ? amount - 1 : top, vertical ? right : amount, vertical ? amount : bottom, color::outline().with_alpha(front.a));
			surf::font::draw(surf::font::esp, vertical ? text_on_left ? left - 1 : right + 1 : amount, vertical ? amount : bottom + 2, value < vmax ? front + 20 : color(255, 0, 0).with_alpha(front.a), vertical ? (text_on_left ? DT_RIGHT : NULL) | DT_VCENTER : DT_CENTER, output, (t)value);
		}
		/*else if (value > vmax)
		{
			if (vertical)
				surf::font::draw(surf::font::esp, left + ((right - left) / 2), top, color(255, 0, 0).with_alpha(front.a), DT_BOTTOM | DT_CENTER, output, (t)value);
			else
				surf::font::draw(surf::font::esp, right + 1, top + ((bottom - top) / 2), color(255, 0, 0).with_alpha(front.a), DT_VCENTER | DT_LEFT, output, (t)value);
		}*/
	}

	template<typename t>
	t interpolate(t base, t to, t factor)
	{
		if (abs(base - to) > factor) return ((to > base) ? (base + factor) : (base - factor));
		else return to;

		return base;
	}

	//float oldx, oldy, dx, dy, lastx, lasty;
	// очень лагает w2s, есп не будет пока w2s не примет христиуанство вновь
	// все таки приняло, за работу
	void draw()
	{
		if (!sets->visuals.bomb_timer && !sets->visuals.esp_filter[0] && !sets->visuals.esp_filter[1] && !sets->visuals.esp_filter[2] && !sets->visuals.esp_filter[3] && !sets->visuals.esp_filter[4] && !sets->visuals.esp_filter[5])
			return;

		if (sets->visuals.esp_filter[0] && sets->visuals.esp_show[4])
			draw_sounds();

		centity* entity;
		iclientnetworkable* networkable;
		clientclass* client_class;
		int class_id;
		bool dormant;
		const char* name;
		matrix3x4_t matrix[128];
		cbox box;
		color p_color;
		static short alpha[64];
		cvector screen;

		for (int id = 0; id < _ent_list->get_highest_entity_index(); id++)
		{
			if (id == global::local_id) continue;			

			entity = _ent_list->get_centity(id);
			if (!entity || entity->get_origin().IsZero() || entity == global::local_observed) continue;

			networkable = entity->get_clientnetworkable();
			if (!networkable) continue;

			dormant = networkable->is_dormant();
			if ((!sets->visuals.fade || id >= 64) && dormant) continue;

			client_class = networkable->get_clientclass();
			if (!client_class) continue;

			class_id = client_class->class_id;
			name = client_class->name;

			if ((class_id == CPlantedC4 && ((!sets->visuals.esp_filter[3] && !sets->visuals.bomb_timer) || !events::bomb_timer::planted)) || (class_id == CCSPlayer && (!sets->visuals.esp_filter[0] || !entity->valid() || (!sets->visuals.friends && entity->get_team() == global::local->get_team()) /*&& (id < 64 && !server::players[id].drawable)*/ || !entity->get_hitbox_matrix(matrix, global::curtime))))
			{
				if (id < 64) alpha[id] = 0;
				continue;
			}

			switch (class_id)
			{
			case CCSPlayer:
			{
				bool visible = box.visible(entity, matrix);
				alpha[id] = sets->visuals.fade ? interpolate<int>(alpha[id], ((!sets->visuals.esp_check[0] && !visible) || dormant) ? 0 : 255, 7) : 255;
				if (alpha[id] == 0 && dormant) continue;
				if (!sets->visuals.esp_check[0] && !visible && (!sets->visuals.fade || alpha[id] == 0)) continue;
				
				box = cbox(/*id, */entity, CCSPlayer);
				if (!box.construct_points()) continue;

				p_color = visible ? entity->get_team() == 2 ? sets->visuals.esp_t : sets->visuals.esp_ct : entity->get_team() != global::local->get_team() ? color::text() : color::disabled();

				player_info_t info;
				if (sets->visuals.esp_show[0] && _engine->get_playerinfo(id, &info))
				{
					surf::font::draw(surf::font::esp, box.centerx, box.top - 2, color::ptext().with_alpha(alpha[id]), DT_CENTER | DT_BOTTOM, str(info.name).substr(0, 25).c_str());
				}

				if (sets->visuals.esp_show[1])
				{
					surf::prim::bordered_box(box.left - 1, box.top - 1, box.right + 1, box.bottom + 1, color::outline().with_alpha(alpha[id]));
					surf::prim::bordered_box(box.left, box.top, box.right, box.bottom, p_color.with_alpha(alpha[id]));
					surf::prim::bordered_box(box.left + 1, box.top + 1, box.right - 1, box.bottom - 1, color::outline().with_alpha(alpha[id]));
				}

				if (sets->visuals.esp_bar[0])
				{
					bar<int>(true, true, box.left - 8, box.top, box.left - 3, box.bottom, entity->get_hp(), 100.f, color(0, 200, 0).with_alpha(alpha[id]), color(0, 50, 0).with_alpha(alpha[id]));
				}

				if (sets->visuals.esp_bar[1] && entity->get_armor() > 0)
				{
					bar<int>(true, false, box.right + 3, box.top, box.right + 8, box.bottom, entity->get_armor(), 100.f, color::text().with_alpha(alpha[id]) + 10, color::disabled().with_alpha(alpha[id]));
				}

				if (sets->visuals.esp_show[2] || sets->visuals.esp_bar[2]) //&& server::players[id].valid)
				{
					auto weapon = entity->get_weapon();
					if (weapon)
					{
						if (sets->visuals.esp_bar[2])
						{
							//auto type = weapon->get_type();
							//auto data = weapon->get_data();
							int ammo = weapon->get_clip1();
							int max_ammo = weapon->get_maxclip1();

							if (ammo != -1 && max_ammo != -1)
							{
								bar<int>(false, false, box.left, box.bottom + 3, box.right, box.bottom + 8, ammo, max_ammo/*min(1, weapon->get_data().iMaxClip1)*/, color(170, 170, 170).with_alpha(alpha[id]), color(50, 50, 50).with_alpha(alpha[id]), "%i");
								box.bottom += 8;
								if (ammo < max_ammo/*weapon->get_data().iMaxClip1*/) box.bottom += 8;
							}
						}

						if (sets->visuals.esp_show[2])
						{
							surf::font::draw(surf::font::esp, box.centerx, box.bottom + 1, color::ptext().with_alpha(alpha[id]), DT_CENTER, weapon->get_name().c_str());
						}
					}
				}

				if (legit::backtrack::draw && sets->legit.backtrack.style[1])
				{
					cvector screen(0, 0, 0);

					if (sets->legit.backtrack.style[3])
					{
						crecord record = records[id][min(13, max(0, (global::cmd->tick_count - sets->legit.backtrack.ticks + 1) % sets->legit.backtrack.ticks))];
						if (record.is_valid(entity) && record.have_matrix && record.hitbox_matrix && w2s(entity->get_hitbox(hitbox_head, record.hitbox_matrix), screen))
						{
							surf::prim::filled_box(screen.x - 3, screen.y - 3, screen.x + 3, screen.y + 3, color::outline().with_alpha(alpha[id]));
							surf::prim::filled_box(screen.x - 2, screen.y - 2, screen.x + 2, screen.y + 2, color(255, 167, 25, alpha[id]));
						}
					}

					if (sets->legit.backtrack.style[2] && best_record.id == id)
					{
						if (best_record.is_valid(entity) && best_record.have_matrix && best_record.hitbox_matrix && w2s(entity->get_hitbox(hitbox_head, best_record.hitbox_matrix), screen))
						{
							surf::prim::filled_box(screen.x - 3, screen.y - 3, screen.x + 3, screen.y + 3, color::outline().with_alpha(alpha[id]));
							surf::prim::filled_box(screen.x - 2, screen.y - 2, screen.x + 2, screen.y + 2, color(30, 255, 30, alpha[id]));
						}
					}
				}

				if (sets->visuals.esp_show[3])
				{
					surf::prim::line(global::screen.right / 2, global::screen.bottom / 2, box.centerx, box.bottom, p_color);
				}

				/*if (server::players[id].drawable_timer > global::curtime)
				{
					server::players[id].drawable = false;
					server::players[id].drawable_timer = 1.f;
				}*/
				continue;
			}
			case CHEGrenade:
			case CSmokeGrenade:
			case CFlashbang:
			{
				if (!sets->visuals.esp_filter[2]) continue;
				
				box = cbox(/*id, */entity, class_id);
				if (!box.construct_points()) continue;

				surf::font::draw(surf::font::esp, box.centerx, box.top - 2, class_id == CHEGrenade ? color(180, 0, 0) : class_id == CSmokeGrenade ? color::disabled() + 80 : color(253, 233, 16), DT_BOTTOM | DT_CENTER, class_id == CHEGrenade ? "he_grenade" : class_id == CSmokeGrenade ? "smoke_grenade" : "flashbang");

				surf::prim::bordered_box(box.left - 1, box.top - 1, box.right + 1, box.bottom + 1, color::outline());
				surf::prim::bordered_box(box.left, box.top, box.right, box.bottom, color::enabled());
				surf::prim::bordered_box(box.left + 1, box.top + 1, box.right - 1, box.bottom - 1, color::outline());
				continue;
			}
			break;
			case CBaseCSGrenadeProjectile:
			{
				if (!sets->visuals.esp_filter[3]) continue;

				bool is_smoke = false, is_flash = false, is_he = false;

				auto model = entity->get_model();
				if (!model) continue;

				auto model_name = _model_info->get_model_name(model);
				if (strstr(model_name, "smoke")) is_smoke = true;
				else if (strstr(model_name, "flash")) is_flash = true;
				else if (strstr(model_name, "frag")) is_he = true;

				box = cbox(/*id, */entity, class_id);
				if (!box.construct_points()) continue;

				const color nade_color = is_flash ? color(253, 233, 16) : is_smoke ? color::disabled() + 80 : is_he ? color(200, 0, 0) : color::lm();

				surf::font::draw(surf::font::esp, box.centerx, box.top - 2, nade_color, DT_BOTTOM | DT_CENTER, is_flash ? "flashbang" : is_smoke ? "smoke_grenade" : is_he ? "hegrenade" : "??????");

				surf::prim::bordered_box(box.left - 1, box.top - 1, box.right + 1, box.bottom + 1, color::outline());
				surf::prim::bordered_box(box.left, box.top, box.right, box.bottom, nade_color);
				surf::prim::bordered_box(box.left + 1, box.top + 1, box.right - 1, box.bottom - 1, color::outline());
				continue;
			}
			break;
			case CC4:
			case CPlantedC4:
			{
				if (events::bomb_timer::explosion_time == 0.f)
				{
					events::bomb_timer::f_exp_time = entity->get_c4_timer_length();//entity->get_c4_blow_time();
					events::bomb_timer::explosion_time = entity->get_c4_blow_time();//global::curtime + events::bomb_timer::f_exp_time;
					//console::write("delta: " + to_str(events::bomb_timer::explosion_time - _globals->curtime) + ", delta2: " + to_str(events::bomb_timer::explosion_time - global::curtime), color::ptext());
				}

				if (!sets->visuals.esp_filter[4])
					continue;

				box = cbox(/*id, */entity, class_id);
				if (!box.construct_points()) continue;

				surf::font::draw(surf::font::esp, box.centerx, box.top - 2, color(220, 0, 0), DT_CENTER | DT_BOTTOM, class_id == CC4 ? "bomb" : "planted_bomb");

				surf::prim::bordered_box(box.left - 1, box.top - 1, box.right + 1, box.bottom + 1, color::outline());
				surf::prim::bordered_box(box.left, box.top, box.right, box.bottom, class_id == CC4 ? color::enabled() : color(200, 0, 0));
				surf::prim::bordered_box(box.left + 1, box.top + 1, box.right - 1, box.bottom - 1, color::outline());

				if (class_id == CPlantedC4)
				{
					if (events::bomb_timer::defuse_time != 0.f)
					{
						bar<float>(false, false, box.left, box.bottom + 21, box.right, box.bottom + 26, (events::bomb_timer::defuse_time - global::curtime), events::bomb_timer::f_def_time, color(0, 0, 200), color(0, 0, 50), (events::bomb_timer::defuse_time - global::curtime) > 0.f ? "%.1f" : "defused");
					}

					bar<float>(false, false, box.left, box.bottom + 3, box.right, box.bottom + 8, (events::bomb_timer::explosion_time - global::curtime), events::bomb_timer::f_exp_time, color(200, 0, 0), color(50, 0, 0), (events::bomb_timer::explosion_time - global::curtime) > 0.f ? "%.1f" : "explode");
				}
				continue;
			}
			break;
			case CBaseAnimating:
			{
				if (!sets->visuals.esp_filter[5] || (sets->visuals.defuser_only_if_need && (!global::local->valid() || global::local->get_team() != 3 || global::local->have_defuser())))
					continue;

				auto model = entity->get_model();
				if (!model) continue;

				auto model_name = _model_info->get_model_name(model);

				if (!strstr(model_name, "defuse")) continue;

				box = cbox(/*id, */entity, class_id);
				if (!box.construct_points()) continue;

				surf::font::draw(surf::font::esp, box.centerx, box.top - 2, color::ptext(), DT_BOTTOM | DT_CENTER, "defuser");

				surf::prim::bordered_box(box.left - 1, box.top - 1, box.right + 1, box.bottom + 1, color::outline());
				surf::prim::bordered_box(box.left, box.top, box.right, box.bottom, color::lm());
				surf::prim::bordered_box(box.left + 1, box.top + 1, box.right - 1, box.bottom - 1, color::outline());
				continue;
			}
			break;
			}

			if (sets->visuals.esp_filter[1] && (strstr(name, "CWeapon") || class_id == CAK47 || class_id == CDEagle))
			{
				box = cbox(/*id, */entity, class_id);
				if (!box.construct_points()) continue;

				//str sname = str(name).substr(7);
				//name = upper_to_lower(sname.c_str(), sname.size());

				string sname = name;
				sname = class_id == CAK47 || class_id == CDEagle ? sname.substr(1) : sname = sname.substr(7);
				transform(sname.begin(), sname.end(), sname.begin(), change_case);
				//name = upper_to_lower(sname.c_str(), strlen(sname.c_str()));

				surf::font::draw(surf::font::esp, box.centerx, box.top - 2, color::ptext(), DT_CENTER | DT_BOTTOM, sname.c_str());

				surf::prim::bordered_box(box.left - 1, box.top - 1, box.right + 1, box.bottom + 1, color::outline());
				surf::prim::bordered_box(box.left, box.top, box.right, box.bottom, color::enabled());
				surf::prim::bordered_box(box.left + 1, box.top + 1, box.right - 1, box.bottom - 1, color::outline());

				if (sets->visuals.esp_bar[3])
				{
					auto weapon = (cweapon*)entity;
					if (weapon)
					{
						int ammo = weapon->get_clip1();
						int max_ammo = weapon->get_maxclip1();

						if (ammo != -1 && max_ammo != -1)
							bar<int>(false, false, box.left, box.bottom + 3, box.right, box.bottom + 7, ammo, max_ammo, color(170, 170, 170), color(50, 50, 50), "%i");
					}
				}
				continue;
			}
		}
	}
}