#pragma once
#include "legit_misc.h"
#include "backtrack.h"
#include "autowall.h"

namespace legit
{
	namespace aimbot
	{
// TODO : заменить на квары ксс
#define m_yaw 0.022f
#define m_pitch 0.022f
#define sensitivity 3.4f

		vector<int> points;
		qangle old_punch(0.f, 0.f, 0.f);
		float kill_delay = 0.f;
		float shot_delay = 0.f;
		float best_fov = 180.f;
		//int best_bone = -1;
		int best_id = -1;
		bool undo = false;
		//bool smac_delay = false;

		short get_priority(int hitbox)
		{
			switch (hitbox)
			{
			case hitbox_head: return 5; break;
			case hitbox_upper_chest: case hitbox_chest: return 4; break;
			case hitbox_pelvis: return 3; break;
			case hitbox_r_up_leg: case hitbox_l_up_leg:
			case hitbox_r_low_leg: case hitbox_l_low_leg:
				return 2; break;
			case hitbox_l_up_arm: case hitbox_r_up_arm:
			case hitbox_l_low_arm: case hitbox_r_low_arm:
				return 1; break;
			}

			return -1;
		}

		void drop()
		{
			old_punch = qangle();
			best_fov = cvar(ragemode).value ? 9999.f : 180.f;
			shot_delay = 0.f;
			//best_bone = -1;
			best_id = -1;
			//smac_delay = false;
			if (cvar(ragemode).value && sets->rage.autoshoot && undo)
				send_mouse(0, 0, MOUSEEVENTF_LEFTUP);
		}

		void check_backtrack(int player_id, bool check_visibility, bool check_penetration, float* fov, int* bone_id, int* tick, centity* entity, qangle* angle, short* best_priority, cvector* point)
		{
			if (sets->legit.backtrack.enabled && !points.empty() && !cvar(antismac).value)
			{
				for (int id = 0; id < sets->legit.backtrack.ticks; id++)
				{
					if (!backtrack::records[player_id][id].valid) continue;
					auto record = &backtrack::records[player_id][id];
					if (abs(global::cmd->tick_count - record->tick) > (sets->legit.backtrack.ticks + 1) ||
						fabs(entity->get_simulation_time() - record->sim_time) >= 0.2f || record->points.empty()) continue;
					float p_fov = -1.f;
					int p_bone = -1;
					qangle p_angle(0.f, 0.f, 0.f);
					auto p_point = record->get_point(check_visibility, check_penetration, entity, &p_fov, &p_bone, &p_angle);
					auto priority = get_priority(p_bone);
					if (priority > *best_priority)
					{
						*point = p_point;
						*fov = p_fov;
						*bone_id = p_bone;
						*tick = id;

						if (angle != nullptr) *angle = p_angle;

						*best_priority = priority;
					}
				}
			}
		}

		cvector get_point(int player_id, bool check_visibility, bool check_penetration, vector<int> check_points, centity* entity, float* fov, int* bone_id, int* tick, matrix3x4_t hitbox_matrix[128], qangle* angle = nullptr)
		{
			cvector point(0.f, 0.f, 0.f);
			const auto viewangle = global::cmd->viewangles + global::local->get_punch() * 2.f;
			const auto eye_pos = global::local->get_eye_pos();

			if (check_points.size() == 1)
			{
				point = entity->get_hitbox(check_points.at(0), hitbox_matrix, global::curtime);
				if (point.IsZero())
				{
					point = cvector(0.f, 0.f, 0.f);
					if (cvar(ragemode).value)
					{
						short best_priority = -1;
						check_backtrack(player_id, check_visibility, check_penetration, fov, bone_id, tick, entity, angle, &best_priority, &point);
					}
					return point;
				}

				bool visible = is_visible(entity, point);
				bool penetrable = false;
				if (check_penetration && !visible)
				{
					weaponinfo_t w;
					get_weapon_info(global::weapon->get_weaponid(), global::weapon->is_silenced(), w);
					penetrable = rage::autowall::is_penetrable(w, eye_pos, point);
				}
				if ((check_visibility && !visible) && !penetrable)
				{
					point = cvector(0.f, 0.f, 0.f);
					if (cvar(ragemode).value)
					{
						short best_priority = -1;
						check_backtrack(player_id, check_visibility, check_penetration, fov, bone_id, tick, entity, angle, &best_priority, &point);
					}
					return point;
				}
				auto calced = calc_angle(eye_pos, point);
				*fov = get_fov(viewangle, calced);
				*bone_id = check_points.at(0);
				if (angle != nullptr) *angle = calced;
				if (cvar(ragemode).value)
				{
					short best_priority = get_priority(*bone_id);
					check_backtrack(player_id, check_visibility, check_penetration, fov, bone_id, tick, entity, angle, &best_priority, &point);
				}
			}
			else if (cvar(ragemode).value)
			{
				short best_priority = -1;

				for (auto id : check_points)
				{
					auto p_point = entity->get_hitbox(id, hitbox_matrix, global::curtime);
					if (p_point.IsZero()) continue;

					bool visible = is_visible(entity, p_point);
					bool penetrable = false;
					if (check_penetration && !visible)
					{
						weaponinfo_t w;
						get_weapon_info(global::weapon->get_weaponid(), global::weapon->is_silenced(), w);
						penetrable = rage::autowall::is_penetrable(w, eye_pos, p_point);
					}
					
					if ((check_visibility && !visible) &&
						!penetrable/*entity->visible(p_point)*/) continue;

					auto calced = calc_angle(eye_pos, p_point);
					auto p_fov = get_fov(viewangle, calced);

					auto priority = get_priority(id);
					if (priority > best_priority)
					{
						point = p_point;
						*fov = p_fov;
						*bone_id = id;
						*tick = -1;

						if (angle != nullptr) *angle = calced;

						best_priority = priority;
					}
				}

				check_backtrack(player_id, check_visibility, check_penetration, fov, bone_id, tick, entity, angle, &best_priority, &point);
			}
			else
			{
				float best_dist = 180.f;
				for (auto id : check_points)
				{
					auto p_point = entity->get_hitbox(id, hitbox_matrix, global::curtime);
					if (p_point.IsZero()) continue;

					if (check_visibility && !is_visible(entity, p_point)/*entity->visible(p_point)*/) continue;

					auto calced = calc_angle(eye_pos, p_point);
					auto p_fov = get_fov(viewangle, calced);

					if (p_fov < best_dist)
					{
						point = p_point;
						best_dist = p_fov;
						*fov = best_dist;
						*bone_id = id;

						if (angle != nullptr) *angle = calced;
					}
				}
			}

			return point;
		}

		inline bool should_aim()
		{
			return (!points.empty() && global::weapon->get_weaponid() != weapon_knife && ((cvar(ragemode).value && sets->rage.autoshoot) || global::cmd->buttons & IN_ATTACK) /*&& best_bone != -1*/ && best_id != -1 && kill_delay <= global::realtime && global::weapon->get_clip1() > 0);
		}

		float humanize(int axis)
		{
			if (sets->legit.aim.humanize[axis] <= 0.01f) return 0.f;

			int range = sets->legit.aim.humanize[axis] * sets->legit.aim.smooth[axis];
			if (range < 2) return 0.f;

			float factor = (float)((float)(rand() % range) / ((float)range * 1000.f));

			return factor >= (1.f - (sets->legit.aim.smooth[axis] * 0.01999f)) ? 0.f : factor;
		}

		inline float get_smooth(int axis)
		{
			return fmin(fmax(1.f - (sets->legit.aim.smooth[axis] * 0.0199f + humanize(axis)), 0.f), 1.f);
		}

		inline qangle angle_to_screen(qangle angle)
		{
			return qangle(-(angle.y / (sensitivity * m_yaw)), angle.x / (sensitivity * m_pitch), 0.f);
		}

		void start()
		{
			if (sets->legit.aim.fov > 0.f)
			{
				misc::set_points(points, sets->legit.aim.hitbox);

				/*if (best_id != -1 && best_fov != 180.f && shot_delay == 0.f && sets->legit.aim.shot_delay != 0.f)
				{
					smac_delay = true;
					shot_delay = global::curtime + sets->legit.aim.shot_delay;
				}*/

				if (cvar(ragemode).value || global::curtime >= kill_delay || sets->legit.aim.kill_delay == 0.f)
					kill_delay = 0.f;

				//if (global::curtime >= shot_delay || sets->legit.aim.shot_delay == 0.f)
				//	smac_delay = false;

				if (!should_aim())
					drop();
			}
		}

		void loop(int id, centity* entity, matrix3x4_t hitbox_matrix[128])
		{
			if (sets->legit.aim.fov == 0.f || points.empty() || should_aim())
				return;

			float fov = 180.f;
			float dist = 9999.f;
			int bone_id = -1;
			int tick = -1;
			auto point = get_point(id, true, cvar(ragemode).value && sets->rage.autowall, points, entity, &fov, &bone_id, &tick, hitbox_matrix);

			if (point.IsZero() || bone_id == -1) return;

			if (id == best_id && fov > sets->legit.aim.fov)
			{
				best_id = -1;
				best_fov = 180.f;
			}

			if (!cvar(ragemode).value && fov <= sets->legit.aim.fov && fov <= best_fov)
			{
				//best_bone = bone_id;
				best_fov = fov;
				best_id = id;
			}
			else if (cvar(ragemode).value && fov <= sets->legit.aim.fov && (dist = (point - global::local->get_origin()).Length()) <= best_fov)
			{
				best_fov = dist;
				best_id = id;
			}
		}

		void end()
		{
			//if (global::key_click[VK_NUMPAD9])
			//	send_mouse(10, 0, MOUSEEVENTF_MOVE);
			//if (global::key_click[VK_NUMPAD8])
			//	send_mouse(0, 10, MOUSEEVENTF_MOVE);

			// TODO : сделай фикс лучше, чем viewangles.x < -87.f || viewangles.x > 87.f
			// P.S. : использовать trace_forward для проверки на врага под прицелом
			// P.P.S: причина - начинает неестественно вертеться
			if (sets->legit.aim.fov <= 0.f || points.empty() || !should_aim() || (!cvar(ragemode).value && (global::cmd->viewangles.x < -87.f || global::cmd->viewangles.x > 87.f))) return;

			global::weapon->update_acc_penalty();

			centity* entity = _ent_list->get_centity(best_id);
			player_info_t pinfo;
			if (!entity->valid() || !_engine->get_playerinfo(best_id, &pinfo))
			{
				drop();
				return;
			}

			matrix3x4_t hitbox_matrix[128];

			if (!entity->get_hitbox_matrix(hitbox_matrix, global::curtime)) return;

			qangle angle(0.f, 0.f, 0.f);
			float fov = 180.f;
			int bone_id = -1;
			int tick = -1;
			//bool visible = entity->hitbox_visible(best_bone, hitbox_matrix);
			//auto point = visible ? 
			//			 entity->get_hitbox(best_bone, hitbox_matrix, global::curtime) :
			//			 get_point(entity, &fov, &best_bone, hitbox_matrix, &angle);
			auto point = get_point(best_id, true, cvar(ragemode).value && sets->rage.autowall, points, entity, &fov, &bone_id, &tick, hitbox_matrix, &angle);
			//if (point.IsZero() || bone_id == -1) return;

			//if (visible) angle = calc_angle(global::local->get_eye_pos(), point);
			//if (angle.IsZero()) return;

			//if (visible) fov = get_fov(global::cmd->viewangles + global::local->get_punch() * 2.f, angle);
			if (fov > sets->legit.aim.fov || point.IsZero() || bone_id == -1 || angle.IsZero())
			{
				drop();
				return;
			}

			auto punch = global::local->get_punch();
			//auto punch_delta = (punch - old_punch);
			//punch_delta.x *= sets->legit.aim.rcs[0];
			//punch_delta.y *= sets->legit.aim.rcs[1];

			if (!punch.IsZero() && punch.Length() < 150.f && (cvar(ragemode).value ||
				(server::local.type != weap_pistol && server::local.type != weap_snip && global::local->get_shots() > 2 &&
				global::key_timer[VK_LBUTTON] != 0.f && (global::realtime - global::key_timer[VK_LBUTTON]) > 0.2f)))
			{
				angle.x -= punch.x * (cvar(ragemode).value ? 2.f : sets->legit.aim.rcs[0]);
				angle.y -= punch.y * (cvar(ragemode).value ? 2.f : sets->legit.aim.rcs[1]);
			}
			//old_punch = punch;
			auto delta = angle - global::cmd->viewangles;

			normalize_angle(delta);

			if (delta.IsZero()) return;

			delta.x *= cvar(ragemode).value ? 1.f : get_smooth(0);
			delta.y *= cvar(ragemode).value ? 1.f : get_smooth(1);

			if (cvar(antismac).value)
			{
				auto screen = angle_to_screen(delta);

				//if (!cvar(ragemode).value || (sets->rage.autoshoot && (global::weapon->next_primary_attack() - global::curtime) <= 0.f))
				send_mouse(screen.x, screen.y, MOUSEEVENTF_MOVE);

				if (cvar(ragemode).value)
				{
					//if (sets->rage.autoscope) send_key(VK_RBUTTON, true);
					if (sets->rage.autoshoot)
					{
						if (global::weapon->next_primary_attack() <= global::curtime && !undo)
						{
							send_mouse(0, 0, MOUSEEVENTF_LEFTDOWN);
							undo = true;
						}
						else
						{
							send_mouse(0, 0, MOUSEEVENTF_LEFTUP);
							undo = false;
						}
					}

					if (sets->rage.autostop)
					{
						send_key(0x57, false);
						send_key(0x41, false);
						send_key(0x53, false);
						send_key(0x44, false);
					}
				}
			}
			else
			{
				if (!cvar(ragemode).value && shot_delay == 0.f && sets->legit.aim.shot_delay != 0.f)
					shot_delay = global::curtime + sets->legit.aim.shot_delay;

				if (shot_delay > global::curtime && global::local->get_shots() == 0) global::cmd->buttons &= ~IN_ATTACK;

				if (!cvar(ragemode).value || ((global::weapon->next_primary_attack() - global::curtime) <= 0 &&
					cvar(ragemode).value &&
					(1.f - (global::weapon->get_spread() * global::weapon->get_inacc() * 10000.f)) >=
					sets->rage.hitchance))
					global::cmd->viewangles += delta;

				normalize_angle(global::cmd->viewangles);

				if (cvar(ragemode).value)
				{
					//if (sets->rage.autoscope) global::cmd->buttons |= IN_ATTACK2;
					if (cvar(ragemode).value && 
						(1.f - (global::weapon->get_spread() * global::weapon->get_inacc() * 10000.f)) >= 
						sets->rage.hitchance && sets->rage.autoshoot && 
						(global::weapon->next_primary_attack() - global::curtime) <= 0.f)
						global::cmd->buttons |= IN_ATTACK;
					backtrack::best_record = tick == -1 ? backtrack::crecord() : backtrack::records[best_id][tick];
					if (sets->rage.autostop) global::cmd->sidemove = global::cmd->forwardmove = global::cmd->upmove = 0;
					if (!sets->rage.silent) _engine->set_viewangles(global::cmd->viewangles);
				}
				else _engine->set_viewangles(global::cmd->viewangles);

				global::cmd->mousedx -= (delta.y / 0.0748f);
				global::cmd->mousedy += (delta.x / 0.0748f);
			}
		}
	}
}