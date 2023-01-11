#pragma once
#include "game shit.h"
#include "global.h"
#include "legit_misc.h"
#include "autowall.h"
//#include "aimbot.h" // TODO: сделать папку для legit, сделать файл misc для set_points/get_point

#define tick_interval _globals->interval_per_tick
#define time_to_ticks(dt) ((int)(0.5f + (float)(dt) / tick_interval))

namespace legit
{
	//CPLAYERINFOMANAGER -> IPLAYERINFO -> CPLAYERINFO -> ABSORIGIN | ABSANGLES//
	namespace backtrack
	{
		vector<int> points;

		struct cvecint
		{
			cvector vector;
			int id;
		};

		struct crecord
		{
			crecord() { this->valid = false; }

			crecord(int id, centity* entity, vector<cvecint> points, cvector origin, int tick)
			{
				this->origin = origin; // GetAbsOrigin
				this->velocity = entity->get_velocity(); // GetAbsVelocity
				this->points = points;

				this->angles = entity->get_angles(); // GetAbsAngles

				if (origin.IsZero() || angles.IsZero())
				{
					this->valid = false;
					return;
				}

				this->tick = tick;
				this->flags = entity->get_flags();
				this->id = id;

				this->sim_time = entity->get_simulation_time(); // GetSimulationTime

				if (sets->visuals.enabled && (sets->legit.backtrack.style[0] || 
					(sets->legit.backtrack.style[1] && sets->visuals.esp_filter[0])) && 
					entity->get_hitbox_matrix(hitbox_matrix, global::curtime)) 
					this->have_matrix = true;

				this->valid = true;
			}
			
			void set()
			{
				if (abs(global::cmd->tick_count - this->tick) > sets->legit.backtrack.ticks)
				{
					*this = crecord();
					return;
				}

				auto entity = _ent_list->get_centity(this->id);

				player_info_t pinfo;
				if (!entity || entity->is_dormant() || !entity->valid() || !_engine->get_playerinfo(this->id, &pinfo))
				{
					*this = crecord();
					return;
				}

				if (time_to_ticks(fabs(entity->get_simulation_time() - this->sim_time)) > sets->legit.backtrack.ticks)
				{
					*this = crecord();
					return;
				}

				// (c) mr-nv <3
				/*entity->set_origin(this->origin); // SetAbsOrigin
				entity->set_velocity(this->velocity); // SetAbsVelocity
				entity->set_flags(this->flags); // мб сменить на форс анимаций или около того? хз
				entity->set_simulation_time(this->sim_time); // SetSimulationTime
				entity->set_angles(this->angles); // SetAbsAngles*/

				global::cmd->tick_count = this->tick;
			}

			inline bool is_valid(centity* entity)
			{
				return (this->valid && (global::cmd->tick_count - this->tick) <= (sets->legit.backtrack.ticks + 1) &&
					time_to_ticks(fabs(entity->get_simulation_time() - this->sim_time)) <= sets->legit.backtrack.ticks);
			}

			cvector get_point(bool check_visibility, bool check_penetration, centity* entity, float* fov, int* bone_id, qangle* angle = nullptr)
			{
				const auto viewangle = global::cmd->viewangles + global::local->get_punch() * 2.f;
				const auto eye_pos = global::local->get_eye_pos();

				for (auto p_point : this->points)
				{
					if (p_point.vector.IsZero()) continue;

					bool visible = this->is_visible(entity, p_point.vector);
					bool penetrable = false;
					if (check_penetration && !visible)
					{
						weaponinfo_t w;
						get_weapon_info(global::weapon->get_weaponid(), global::weapon->is_silenced(), w);
						penetrable = rage::autowall::is_penetrable(w, eye_pos, p_point.vector);
					}
					if (check_visibility && !visible && !penetrable) continue;

					auto calced = calc_angle(eye_pos, p_point.vector);
					auto p_fov = get_fov(viewangle, calced);

					*fov = p_fov;
					*bone_id = p_point.id;

					if (angle != nullptr) *angle = calced;
					return p_point.vector;
				}
			}

		private:
			bool is_visible(centity* entity, cvector to)
			{
				if (to.IsZero()) return false;

				cvector from = global::local_observed->get_eye_pos();
				if (from.IsZero()) return false;

				itracefilter filter;
				filter.skip = global::local_observed;
				filter.target = entity;

				ray_t ray; ray.Init(from, to);

				trace_t trace;

				_engine_trace->trace_ray(ray, MASK_SOLID/*MASK_ALL*/, &filter, &trace);

				return (!trace.allsolid && (trace.m_pEnt == entity || trace.endpos.DistToSqr(from) > to.DistToSqr(from)));
			}

		public:
			vector<cvecint> points;
			cvector origin, velocity;
			qangle angles;
			int tick, flags, id;
			float sim_time;
			matrix3x4_t hitbox_matrix[128];
			bool have_matrix = false;
			bool valid = false;
		};

		crecord records[64][14];
		crecord best_record = crecord();
		float best_fov = 180.f;
		bool draw = false;

		void get_points(centity* entity, matrix3x4_t hitbox_matrix[128], vector<cvecint>& p_points)
		{
			for (auto id : points)
			{
				p_points.push_back({ entity->get_hitbox(id, hitbox_matrix, global::curtime), id });
			}
		}

		void/*cvector*/ get_point(centity* entity, vector<cvecint> p_points, float* fov, cvector eye_pos, qangle viewangle)
		{
			//cvector best_point = cvector();

			for (auto point : p_points)
			{
				auto p_fov = get_fov(viewangle, calc_angle(eye_pos, point.vector));

				if (p_fov < *fov)
				{
					//best_point = point;
					*fov = p_fov;
				}
			}

			//return best_point;
		}

		void add(int id, centity* entity, vector<cvecint> p_points, cvector origin)
		{
			crecord new_record(id, entity, p_points, origin, global::cmd->tick_count);
			if (!new_record.valid) return;
			records[id][global::cmd->tick_count % sets->legit.backtrack.ticks] = new_record;
		}

		void start()
		{
			if (sets->legit.backtrack.enabled && !cvar(antismac).value)
			{
				misc::set_points(points, sets->legit.backtrack.hitbox);
				backtrack::best_record.valid = false;
				backtrack::best_fov = 9999.f;
			}
		}

		void find(int id, centity* entity, cvector eye_pos, qangle viewangle)
		{
			for (int tick = 0; tick < sets->legit.backtrack.ticks; tick++)
			{
				if (!records[id][tick].valid) continue;

				auto* record = &records[id][tick];

				if (abs(global::cmd->tick_count - record->tick) > (sets->legit.backtrack.ticks + 1))
				{
					*record = crecord();
					return;
				}

				if (fabs(entity->get_simulation_time() - record->sim_time) >= 0.2f)
				{
					*record = crecord();
					return;
				}

				if (global::weapon->get_weaponid() == weapon_knife)
				{
					auto distance = cvector(global::local->get_origin() - record->origin).Length2D();

					if (distance <= best_fov)
					{
						best_fov = distance;
						best_record = *record;
					}
				}
				else
				{
					if (record->points.empty())
					{
						*record = crecord();
						continue;
					}

					//auto fov = get_fov(viewangle, calc_angle(eye_pos, record->point));
					float fov = 180.f;
					get_point(entity, record->points, &fov, eye_pos, viewangle);

					if (fov <= best_fov)
					{
						best_fov = fov;
						best_record = *record;
					}
				}
			}
		}

		void loop(int id, centity* entity, matrix3x4_t hitbox_matrix[128])
		{
			if (!sets->legit.backtrack.enabled || points.empty() || cvar(antismac).value)
				return;

			//auto hitbox = entity->get_hitbox(hitbox_head, hitbox_matrix, global::curtime);
			const auto eye_pos = global::local->get_eye_pos();
			const auto viewangle = global::cmd->viewangles + global::local->get_punch() * 2.f;
			float fov = 180.f;
			//int bone_id = -1;
			vector<cvecint> p_points;

			if (global::weapon->get_weaponid() != weapon_knife)
			{
				get_points(entity, hitbox_matrix, p_points);
				get_point(entity, p_points, &fov, eye_pos, viewangle);
			}

			const auto origin = entity->get_origin();

			backtrack::add(id, entity, p_points, origin);
			backtrack::find(id, entity, eye_pos, viewangle);

			const float new_best_fov = global::weapon->get_weaponid() == weapon_knife ? cvector(global::local->get_origin() - origin).Length2D() : fov;

			if (new_best_fov <= backtrack::best_fov)
			{
				best_record = crecord();
				best_fov = new_best_fov;
			}
		}

		void end()//писька
		{
			if (!sets->legit.backtrack.enabled || points.empty() || cvar(antismac).value)
			{
				draw = false;
				return;
			}

			draw = sets->legit.backtrack.style[0] || sets->legit.backtrack.style[1];

			if (!best_record.valid) return;

			if (global::weapon->get_weaponid() == weapon_knife)
			{
				if (!(global::cmd->buttons & IN_ATTACK) || global::weapon->next_primary_attack() > global::curtime)
					if (!(global::cmd->buttons & IN_ATTACK2) || global::weapon->next_secondary_attack() > global::curtime)
						return;
			}
			else if (global::weapon->get_clip1() <= 0 ||  !(global::cmd->buttons & IN_ATTACK) || global::weapon->next_primary_attack() > global::curtime)
				return;

			best_record.set();
		}
	}
}