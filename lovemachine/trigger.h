#pragma once
#include "game classes.h"
#include "trace shit.h"
#include "settings.h"
#include "math.h"

namespace legit
{
	namespace trigger
	{
		bool undo = false;
		float timer = 0.f;

		// TODO: сделать на trace.hitgroup??
		bool valid_hitbox(int hitbox)
		{
			switch (hitbox)
			{
			case 11: case 12: /*голова и шея*/
				return sets->legit.trigger.hitbox[0];
				break;
			case 10: /*вся грудь*/
				return sets->legit.trigger.hitbox[1];
				break;
			case 0: case 9: /*пах и живот*/
				return sets->legit.trigger.hitbox[2];
				break;
			case 13: case 14: /*левая рука*/
			case 16: case 17: /*правая рука*/
				return sets->legit.trigger.hitbox[3];
				break;
			case 1: case 2: case 4: /*левая нога*/
			case 5: case 6: case 8: /*правая нога*/
				return sets->legit.trigger.hitbox[4];
				break;
			}
		}

		// я не знаю как это назвать
		void trace_forward(qangle viewangle, cvector position, centity* skip, trace_t* trace, float distance, unsigned int mask = MASK_ALL)
		{
			cvector pos(0.f, 0.f, 0.f);
			anglevectors(viewangle, &pos);
			if (pos.IsZero()) return;
			pos *= distance;
			pos += position;

			itracefilter filter;
			filter.skip = skip;

			ray_t ray; ray.Init(position, pos);

			_engine_trace->trace_ray(ray, /*MASK_SOLID*/mask, &filter, trace);
		}

		void start()
		{
			if (!sets->legit.trigger._enabled || global::weapon->get_weaponid() == weapon_knife)
				return;

			if (undo)
			{
				send_mouse(0, 0, MOUSEEVENTF_LEFTUP);
				undo = false;
			}

			//if (timer <= global::curtime)
			//	timer = 0.f;
		}

		// TODO: улучшить distance (разная дальность поражения у каждого оружия)
		void end()
		{
			bool bind = sets->legit.trigger.bind.is();
			if (!sets->legit.trigger._enabled || (bind != bind_no_key && bind != bind_true) || global::weapon->get_weaponid() == weapon_knife || global::weapon->next_primary_attack() > global::curtime)
				return;

			trace_t trace;
			trace_forward(global::cmd->viewangles + (global::local->get_punch() * 2.f), global::local->get_eye_pos(), global::local, &trace, 8192.f);

			if (!trace.m_pEnt || trace.m_pEnt->get_class_id() != CCSPlayer || (!sets->legit.friends && trace.m_pEnt->get_team() == global::local->get_team()) || trace.allsolid || !valid_hitbox(trace.hitbox))
			{
				timer = 0.f;
				return;
			}

			if (timer == 0.f && sets->legit.trigger.delay != 0.f)
				timer = global::curtime + sets->legit.trigger.delay;

			if (timer <= global::curtime)
			{
				if (cvar(antismac).value)
				{
					send_mouse(0, 0, MOUSEEVENTF_LEFTDOWN);
					undo = true;
				}
				else global::cmd->buttons |= IN_ATTACK;
			}
		}

		void draw()
		{
			bool bind = sets->legit.trigger.bind.is();
			if (!sets->legit.trigger._enabled || (bind != bind_no_key && bind != bind_true))
				return;

			font::draw(font::hitmarker_big, global::screen.right - 5, 35, color(255, 0, 0), DT_TOP | DT_RIGHT, "TRIGGER");
		}
	}
}