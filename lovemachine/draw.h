#pragma once
#include "game shit.h"
#include "game def's.h"
#include "game classes.h"
#include "events.h"
#include "settings.h"
#include "surface.h"
#include "trigger.h"
#include "d3d.h"

namespace misc
{
	namespace draw
	{
		struct element
		{
			Vector point;
			float time;
		};

		bool press = false;
		deque<element> drawlist;

		void run()
		{
			if (sets->misc.draw_start.is() != bind_true)
				return;

			trace_t trace;
			legit::trigger::trace_forward(global::cmd->viewangles, global::local->get_eye_pos(), global::local, &trace, 8192.f, MASK_SOLID);

			drawlist.push_back({ trace.endpos, _globals->curtime + sets->misc.draw_time });
		}

		void draw()
		{
			if (drawlist.empty())
				return;

			for (int i = 0; i < drawlist.size(); i++)
			{
				if (drawlist.at(i).time < _globals->curtime)
				{
					drawlist.erase(drawlist.begin() + i);
					i--;
					continue;
				}

				Vector start = drawlist.at(i).point;
				Vector end = i < (drawlist.size() - 1) ? drawlist.at(i + 1).point : start;
				switch (sets->misc.draw_mode)
				{
				case 0:
					Vector _start, _end; if (d3d::w2s(start, _start) && d3d::w2s(end, _end))
					{
						surf::prim::line(start.x, start.y, end.x, end.y,
							sets->misc.draw_color.with_alpha(/*((drawlist.at(i).time - _globals->curtime) / sets->misc.draw_time) * */ 255.f));
					}
					break;
				}
			}
		}

		void clear(bool key)
		{
			if (key && sets->misc.draw_clear.is() == bind_true || !key)
			{
				drawlist.clear();
			}
		}
	}
}