#pragma once
#include "game classes.h"
#include "math.h"
#include "settings.h"
#include "utilities.h"

namespace legit
{
	namespace misc
	{
		void set_points(vector<int>& p_points, bool p_array[4])
		{
			p_points.clear();

			if (p_array[0])
				p_points.push_back(hitbox_head);
			if (p_array[1])
			{
				p_points.push_back(hitbox_upper_chest);
				p_points.push_back(hitbox_chest);
			}
			if (p_array[2])
				p_points.push_back(hitbox_pelvis);
			if (p_array[4])
			{
				p_points.push_back(hitbox_r_up_leg);
				p_points.push_back(hitbox_l_up_leg);
				p_points.push_back(hitbox_r_low_leg);
				p_points.push_back(hitbox_l_low_leg);
			}
			if (p_array[3])
			{
				p_points.push_back(hitbox_l_up_arm);
				p_points.push_back(hitbox_r_up_arm);
				p_points.push_back(hitbox_l_low_arm);
				p_points.push_back(hitbox_r_low_arm);
			}
		}
	}
}