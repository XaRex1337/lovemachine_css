#pragma once
#include "game shit.h"
#include "game classes.h"
#include "math.h"
#include "settings.h"
#include "global.h"
#include "trigger.h"

namespace legit
{
	namespace knifebot
	{
		bool undo = false;
		int last_attack = 0;

		inline int get_attack(centity* entity)
		{
			// без брони левой 20 правой 65
			// без брони в спину правой 195

			if (entity->get_armor() == 0)
			{
				if (entity->get_hp() <= 20)
					return IN_ATTACK;
				else
					return IN_ATTACK2;
			}
			else if (entity->get_hp() <= 17)
				return IN_ATTACK;
			else
				return IN_ATTACK2;

			return IN_ATTACK2;
		}

		void start()
		{
			if (undo && last_attack != 0)
			{
				send_mouse(0, 0, last_attack == IN_ATTACK ? MOUSEEVENTF_LEFTUP : MOUSEEVENTF_RIGHTUP);
				undo = false;
				last_attack = 0;
			}
		}

		void end()
		{
			if (!sets->legit.knifebot || global::weapon->get_weaponid() != weapon_knife) return;

			int button = 0;

			// задумка заключается в том, чтобы трейсить по несколько лучей влево и вправо, таким образом находя ближайшего к нам
			// +так лучше рассчитывается дистанция до игрока
			for (int i = 0; i < 5; i++)
				for (int j = 0; j < 3; j++)
				{
					float yaw_correction = 20 - (10 * i);
					float pitch_correction = 10 - (10 * j);
					qangle viewangle = qangle(clamp(-89.f, global::cmd->viewangles.x + pitch_correction, 89.f), global::cmd->viewangles.y + yaw_correction, 0.f);
					normalize_angle(viewangle);
					trace_t trace;
					trigger::trace_forward(viewangle, global::local->get_eye_pos(), global::local, &trace, cvar(kb_range).value + 10.f);

					if (!trace.m_pEnt->valid() || trace.m_pEnt->is_dormant() || trace.m_pEnt->get_class_id() != CCSPlayer || trace.allsolid || (!sets->legit.friends && trace.m_pEnt->get_team() == global::local->get_team()))
						continue;

					const auto distance = (trace.endpos - trace.startpos).Length();//trace.endpos.DistToSqr(global::local->get_eye_pos());

					button = get_attack(trace.m_pEnt);

					if ((button == IN_ATTACK2 && distance >= cvar(kb_range).value) || (button == IN_ATTACK && distance >= (cvar(kb_range).value + 10.f)))
					{
						button = 0;
						continue;
					}

					break;
				}

			if (button == 0 || (button == IN_ATTACK && global::weapon->next_primary_attack() > global::curtime) ||
				(button == IN_ATTACK2 && global::weapon->next_secondary_attack() > global::curtime))
				return;

			if (cvar(antismac).value)
			{
				send_mouse(0, 0, button == IN_ATTACK ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_RIGHTDOWN);
				undo = true;
				last_attack = button;
			}
			else
				global::cmd->buttons |= button;

			/*bool visible = entity->hitbox_visible(0, hitbox_matrix);

			if (visible)
			{
				if (global::weapon->get_weaponid() == weapon_knife && global::weapon->next_secondary_attack() <= global::curtime)
				{
					auto distance = cvector(global::local->get_origin() - entity->get_origin()).Length2D();
					if (distance <= cvar(kb_range).value)
					{
						auto angle = calc_angle(global::local->get_eye_pos(), entity->get_hitbox(0, hitbox_matrix, global::curtime));
						auto fov = get_fov(global::cmd->viewangles, angle);
						if (fov <= 80.f) // 89.f
						{
							if (cvar(antismac).value)
							{
								_engine->clientcmd_unrestricted("+attack2");
								undo = true;
							}
							else global::cmd->buttons |= IN_ATTACK2;
						}
					}
				}
				else if (sets->legit.knifebot && undo)
				{
					_engine->clientcmd_unrestricted("-attack2");
					undo = false;
				}
			}*/
		}
	}
}