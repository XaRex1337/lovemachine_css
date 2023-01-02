#pragma once
#include "knifebot.h"
#include "backtrack.h"
#include "aimbot.h"
#include "trigger.h"

namespace legit
{
	void start()
	{
		if (!*global::lock_cursor || !global::local->valid() || !sets->legit.enabled || (server::local.type == weap_max || (server::local.type == weap_misc && global::weapon->get_weaponid() != weapon_knife))) return;

		backtrack::start();
		aimbot::start();
		knifebot::start();
		trigger::start();
	}

	void loop(int id, centity* entity)
	{
		if (!*global::lock_cursor || !global::local->valid() || !sets->legit.enabled || (server::local.type == weap_max || (server::local.type == weap_misc && global::weapon->get_weaponid() != weapon_knife))) return;

		if (!entity->valid())
		{
			ZeroMemory(backtrack::records[id], sizeof(backtrack::records[id]));
			return;
		}

		if (!sets->legit.friends && entity->get_team() == global::local->get_team())
			return;

		matrix3x4_t hitbox_matrix[128];
		if (!entity->get_hitbox_matrix(hitbox_matrix, global::curtime))
		{
			ZeroMemory(backtrack::records[id], sizeof(backtrack::records[id]));
			return;
		}

		//knifebot::loop(entity, hitbox_matrix);
		backtrack::loop(id, entity, hitbox_matrix);
		aimbot::loop(id, entity, hitbox_matrix);
	}

	void end()
	{
		if (!*global::lock_cursor || !global::local->valid() || !sets->legit.enabled || server::local.type == weap_max || (server::local.type == weap_misc && global::weapon->get_weaponid() != weapon_knife))
		{
			backtrack::draw = false;
			return;
		}

		aimbot::end();
		backtrack::end();
		knifebot::end();
		trigger::end();
	}
}