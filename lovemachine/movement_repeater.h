#pragma once
#include "game classes.h"
#include "math.h"
#include "settings.h"
#include "utilities.h"

namespace movement
{
	centity* player = nullptr;
	int flags = 0;
	int old_shots = 0;
	cusercmd* tick_info = new cusercmd();
	bool recording = false, playing = false;
	deque<cusercmd*> ticks;

	void record_tick()
	{
		ZeroMemory(tick_info, sizeof(tick_info));

		player = global::local->get_spec_player();
		flags = player->get_flags();

		if (!(flags & FL_ONGROUND) && !ticks.empty())
		{
			ticks.at(ticks.size() - 1)->buttons |= IN_JUMP;
		}

		if (player->get_shots() > old_shots)
		{
			tick_info->buttons |= IN_ATTACK;
		}

		tick_info->viewangles = player->get_angles();

		tick_info->forwardmove = cos(deg2rad(tick_info->viewangles.y)) * OldForward + cos(deg2rad(tick_info->viewangles.y + 90.f)) * OldSideMove;
		tick_info->sidemove = sin(deg2rad(tick_info->viewangles.y)) * OldForward + sin(deg2rad(tick_info->viewangles.y + 90.f)) * OldSideMove;

		old_shots = player->get_shots();
	}
}