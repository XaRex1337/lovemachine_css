#pragma once
#include "definitions.h"
#include "settings.h"
#include "cvars.h"

template<typename t>
struct an_var
{
	an_var(t* value, string category, string name)
	{
		this->value = value;
		this->category = category;
		this->name = name;
	}

	void write(string path)
	{
		WritePrivateProfileStringA(category.c_str(), name.c_str(), to_str(*value).c_str()/*std::is_same<t, bool>() ? *value ? "true" : "false" : std::is_same<t, float>() ? to_str((float)*value).c_str() : to_str((int)*value).c_str()*/, path.c_str());
	}

	void parse(string path)
	{
		//char* sz_value = (char*)"";
		char* sz_value = new char[32];
		GetPrivateProfileStringA(category.c_str(), name.c_str(), "", sz_value, 32, path.c_str());
		*value = std::is_same<t, float>() ? atof(sz_value) : atoi(sz_value);//atof(sz_value);
		//*value = std::is_same<t, bool>() ? sz_value == "true" : std::is_same<t, float>() ? atof(sz_value) : atoi(sz_value);
	}

	t* value;
	string category, name;
};

namespace configs
{
	deque<an_var<int>> ints;
	deque<an_var<bool>> bools;
	deque<an_var<float>> floats;

	void on_inject()
	{
		CreateDirectoryW(L"C:/lovemachine", NULL);
		CreateDirectoryW(L"C:/lovemachine/configs", NULL);

		bools.push_back(an_var<bool>(&sets->info.opened, "information", "opened"));
		ints.push_back(an_var<int>(&sets->info.x, "information", "x"));
		ints.push_back(an_var<int>(&sets->info.y, "information", "y"));

		bools.push_back(an_var<bool>(&sets->spec.opened, "spectators", "opened"));
		ints.push_back(an_var<int>(&sets->spec.x, "spectators", "x"));
		ints.push_back(an_var<int>(&sets->spec.y, "spectators", "y"));

		bools.push_back(an_var<bool>(&sets->legit.enabled, "legit.main", "enabled"));
		bools.push_back(an_var<bool>(&sets->legit.friends, "legit.main", "friends"));
		bools.push_back(an_var<bool>(&sets->legit.knifebot, "legit.main", "knifebot"));

		bools.push_back(an_var<bool>(&sets->legit.backtrack.enabled, "legit.backtrack", "enabled"));
		bools.push_back(an_var<bool>(&sets->legit.backtrack.hitbox[0], "legit.backtrack", "hitbox_head"));
		bools.push_back(an_var<bool>(&sets->legit.backtrack.hitbox[1], "legit.backtrack", "hitbox_chest"));
		bools.push_back(an_var<bool>(&sets->legit.backtrack.hitbox[2], "legit.backtrack", "hitbox_pelvis"));
		bools.push_back(an_var<bool>(&sets->legit.backtrack.hitbox[3], "legit.backtrack", "hitbox_arms"));
		bools.push_back(an_var<bool>(&sets->legit.backtrack.hitbox[4], "legit.backtrack", "hitbox_legs"));
		bools.push_back(an_var<bool>(&sets->legit.backtrack.style[0], "legit.backtrack", "style_chams"));
		bools.push_back(an_var<bool>(&sets->legit.backtrack.style[1], "legit.backtrack", "style_dots"));
		bools.push_back(an_var<bool>(&sets->legit.backtrack.style[2], "legit.backtrack", "style_best"));
		bools.push_back(an_var<bool>(&sets->legit.backtrack.style[3], "legit.backtrack", "style_last"));
		ints.push_back(an_var<int>(&sets->legit.backtrack.ticks, "legit.backtrack", "ticks"));

		bools.push_back(an_var<bool>(&sets->legit.aim.hitbox[0], "legit.aim", "hitbox_head"));
		bools.push_back(an_var<bool>(&sets->legit.aim.hitbox[1], "legit.aim", "hitbox_chest"));
		bools.push_back(an_var<bool>(&sets->legit.aim.hitbox[2], "legit.aim", "hitbox_pelvis"));
		bools.push_back(an_var<bool>(&sets->legit.aim.hitbox[3], "legit.aim", "hitbox_arms"));
		bools.push_back(an_var<bool>(&sets->legit.aim.hitbox[4], "legit.aim", "hitbox_legs"));
		floats.push_back(an_var<float>(&sets->legit.aim.fov, "legit.aim", "fov"));
		floats.push_back(an_var<float>(&sets->legit.aim.smooth[0], "legit.aim", "smooth_x"));
		floats.push_back(an_var<float>(&sets->legit.aim.smooth[1], "legit.aim", "smooth_y"));
		floats.push_back(an_var<float>(&sets->legit.aim.rcs[0], "legit.aim", "rcs_x"));
		floats.push_back(an_var<float>(&sets->legit.aim.rcs[1], "legit.aim", "rcs_y"));
		floats.push_back(an_var<float>(&sets->legit.aim.humanize[0], "legit.aim", "humanize_x"));
		floats.push_back(an_var<float>(&sets->legit.aim.humanize[1], "legit.aim", "humanize_y"));
		floats.push_back(an_var<float>(&sets->legit.aim.kill_delay, "legit.aim", "kill_delay"));
		floats.push_back(an_var<float>(&sets->legit.aim.shot_delay, "legit.aim", "shot_delay"));

		bools.push_back(an_var<bool>(&sets->legit.trigger._enabled, "legit.trigger", "enabled"));
		bools.push_back(an_var<bool>(&sets->legit.trigger.bind.hold, "legit.trigger", "bind.hold"));
		ints.push_back(an_var<int>(&sets->legit.trigger.bind.key, "legit.trigger", "bind.key"));
		bools.push_back(an_var<bool>(&sets->legit.trigger.hitbox[0], "legit.trigger", "hitbox_head"));
		bools.push_back(an_var<bool>(&sets->legit.trigger.hitbox[1], "legit.trigger", "hitbox_chest"));
		bools.push_back(an_var<bool>(&sets->legit.trigger.hitbox[2], "legit.trigger", "hitbox_pelvis"));
		bools.push_back(an_var<bool>(&sets->legit.trigger.hitbox[3], "legit.trigger", "hitbox_arms"));
		bools.push_back(an_var<bool>(&sets->legit.trigger.hitbox[4], "legit.trigger", "hitbox_legs"));
		floats.push_back(an_var<float>(&sets->legit.trigger.delay, "legit.trigger", "delay"));

		//bools.push_back(an_var<bool>(&sets->rage.autoscope, "rage.other", "autoscope"));
		bools.push_back(an_var<bool>(&sets->rage.autowall, "rage.other", "autowall"));
		bools.push_back(an_var<bool>(&sets->rage.autoshoot, "rage.other", "autoshoot"));
		bools.push_back(an_var<bool>(&sets->rage.autostop, "rage.other", "autostop"));
		bools.push_back(an_var<bool>(&sets->rage.silent, "rage.other", "silent"));
		floats.push_back(an_var<float>(&sets->rage.hitchance, "rage.other", "hitchance"));

		bools.push_back(an_var<bool>(&sets->visuals.enabled, "visuals.main", "enabled"));
		bools.push_back(an_var<bool>(&sets->visuals.friends, "visuals.main", "friends"));
		ints.push_back(an_var<int>(&sets->visuals.chams, "visuals.chams", "type"));
		ints.push_back(an_var<int>(&sets->visuals.chams_t.r, "visuals.chams", "chams_t.r"));
		ints.push_back(an_var<int>(&sets->visuals.chams_t.g, "visuals.chams", "chams_t.g"));
		ints.push_back(an_var<int>(&sets->visuals.chams_t.b, "visuals.chams", "chams_t.b"));
		ints.push_back(an_var<int>(&sets->visuals.chams_ct.r, "visuals.chams", "chams_ct.r"));
		ints.push_back(an_var<int>(&sets->visuals.chams_ct.g, "visuals.chams", "chams_ct.g"));
		ints.push_back(an_var<int>(&sets->visuals.chams_ct.b, "visuals.chams", "chams_ct.b"));
		bools.push_back(an_var<bool>(&sets->visuals.chams_style[0], "visuals.chams", "style_invis"));
		bools.push_back(an_var<bool>(&sets->visuals.chams_style[1], "visuals.chams", "style_shine"));
		bools.push_back(an_var<bool>(&sets->visuals.chams_style[2], "visuals.chams", "style_glow"));
		bools.push_back(an_var<bool>(&sets->visuals.chams_style[3], "visuals.main", "style_asus"));
		bools.push_back(an_var<bool>(&sets->visuals.other_chams[0], "visuals.chams", "chams_weapon"));
		bools.push_back(an_var<bool>(&sets->visuals.other_chams[1], "visuals.chams", "chams_dropped"));
		bools.push_back(an_var<bool>(&sets->visuals.other_chams[2], "visuals.chams", "asus_props"));
		bools.push_back(an_var<bool>(&sets->visuals.other_style[0], "visuals.chams", "other_chams"));
		bools.push_back(an_var<bool>(&sets->visuals.other_style[1], "visuals.chams", "other_color"));
		bools.push_back(an_var<bool>(&sets->visuals.other_style[2], "visuals.chams", "other_invis"));
		bools.push_back(an_var<bool>(&sets->visuals.other_style[3], "visuals.chams", "other_shine"));
		bools.push_back(an_var<bool>(&sets->visuals.other_style[4], "visuals.chams", "other_glow"));
		bools.push_back(an_var<bool>(&sets->visuals.other_style[5], "visuals.chams", "other_asus"));
		ints.push_back(an_var<int>(&sets->visuals.crosshair, "visuals.other", "crosshair"));
		bools.push_back(an_var<bool>(&sets->visuals.esp_filter[0], "visuals.esp", "player"));
		bools.push_back(an_var<bool>(&sets->visuals.esp_filter[1], "visuals.esp", "dropped_weapon"));
		bools.push_back(an_var<bool>(&sets->visuals.esp_filter[2], "visuals.esp", "drop_nade"));
		bools.push_back(an_var<bool>(&sets->visuals.esp_filter[3], "visuals.esp", "throw_nade"));
		bools.push_back(an_var<bool>(&sets->visuals.esp_filter[4], "visuals.esp", "bomb"));
		bools.push_back(an_var<bool>(&sets->visuals.esp_filter[5], "visuals.esp", "defusers"));
		bools.push_back(an_var<bool>(&sets->visuals.esp_show[0], "visuals.esp", "name"));
		bools.push_back(an_var<bool>(&sets->visuals.esp_show[1], "visuals.esp", "box"));
		bools.push_back(an_var<bool>(&sets->visuals.esp_show[2], "visuals.esp", "player_weapon"));
		bools.push_back(an_var<bool>(&sets->visuals.esp_show[3], "visuals.esp", "lines"));
		bools.push_back(an_var<bool>(&sets->visuals.esp_show[4], "visuals.esp", "steps"));
		bools.push_back(an_var<bool>(&sets->visuals.esp_show[5], "visuals.esp", "shots"));
		bools.push_back(an_var<bool>(&sets->visuals.esp_bar[0], "visuals.esp", "healthbar"));
		bools.push_back(an_var<bool>(&sets->visuals.esp_bar[1], "visuals.esp", "armorbar"));
		bools.push_back(an_var<bool>(&sets->visuals.esp_bar[2], "visuals.esp", "player_ammobar"));
		bools.push_back(an_var<bool>(&sets->visuals.esp_bar[3], "visuals.esp", "weapon_ammobar"));
		bools.push_back(an_var<bool>(&sets->visuals.esp_check[0], "visuals.esp", "invisible"));
		bools.push_back(an_var<bool>(&sets->visuals.esp_check[1], "visuals.esp", "on_sound"));
		bools.push_back(an_var<bool>(&sets->visuals.defuser_only_if_need, "visuals.esp", "defuser_only_if_need"));
		ints.push_back(an_var<int>(&sets->visuals.esp_t.r, "visuals.esp", "esp_t.r"));
		ints.push_back(an_var<int>(&sets->visuals.esp_t.g, "visuals.esp", "esp_t.g"));
		ints.push_back(an_var<int>(&sets->visuals.esp_t.b, "visuals.esp", "esp_t.b"));
		ints.push_back(an_var<int>(&sets->visuals.esp_ct.r, "visuals.esp", "esp_ct.r"));
		ints.push_back(an_var<int>(&sets->visuals.esp_ct.g, "visuals.esp", "esp_ct.g"));
		ints.push_back(an_var<int>(&sets->visuals.esp_ct.b, "visuals.esp", "esp_ct.b"));
		bools.push_back(an_var<bool>(&sets->visuals.fade, "visuals.esp", "fade"));
		//bools.push_back(an_var<bool>(&sets->visuals.fancy_w2s, "visuals.esp", "fancy_w2s"));

		//TODO : info_style

		ints.push_back(an_var<int>(&sets->visuals.ak47_skin, "visuals.other", "ak47_skin"));
		ints.push_back(an_var<int>(&sets->visuals.deagle_skin, "visuals.other", "deagle_skin"));
		ints.push_back(an_var<int>(&sets->visuals.knife_skin, "visuals.other", "knife_skin"));
		bools.push_back(an_var<bool>(&sets->visuals.other_skins[0], "visuals.other", "skin_for_c4"));
		bools.push_back(an_var<bool>(&sets->visuals.other_skins[1], "visuals.other", "skins_for_grenades"));
		bools.push_back(an_var<bool>(&sets->visuals.hitmarker, "visuals.other", "hitmarker"));
		bools.push_back(an_var<bool>(&sets->visuals.bomb_timer, "visuals.other", "bomb_timer"));
		bools.push_back(an_var<bool>(&sets->visuals.remove[0], "visuals.other", "remove_smoke"));
		bools.push_back(an_var<bool>(&sets->visuals.remove[1], "visuals.other", "remove_flash"));

		bools.push_back(an_var<bool>(&sets->misc.autopistol, "misc", "autopistol"));
		bools.push_back(an_var<bool>(&sets->misc.autojump, "misc", "autojump"));
		bools.push_back(an_var<bool>(&sets->misc.autostrafer, "misc", "autostrafer"));
		ints.push_back(an_var<int>(&sets->misc.aj_percent, "misc", "aj_percent"));
		bools.push_back(an_var<bool>(&sets->misc.killshot, "misc", "killshot"));
		bools.push_back(an_var<bool>(&sets->misc.fl_spam_always, "misc", "fl_spam_always"));
		bools.push_back(an_var<bool>(&sets->misc.fl_spam.hold, "misc", "fl_spam.hold"));
		ints.push_back(an_var<int>(&sets->misc.fl_spam.key, "misc", "fl_spam.key"));
		bools.push_back(an_var<bool>(&sets->misc.lag_spam.hold, "misc", "lag_spam.hold"));
		ints.push_back(an_var<int>(&sets->misc.lag_spam.key, "misc", "lag_spam.key"));
		ints.push_back(an_var<int>(&sets->misc.lag_factor, "misc", "lag_factor"));
		bools.push_back(an_var<bool>(&sets->misc.airstuck.hold, "misc", "airstuck.hold"));
		ints.push_back(an_var<int>(&sets->misc.airstuck.key, "misc", "airstuck.key"));
		bools.push_back(an_var<bool>(&sets->misc.slowmotion.hold, "misc", "slowmotion.hold"));
		ints.push_back(an_var<int>(&sets->misc.slowmotion.key, "misc", "slowmotion.key"));
		ints.push_back(an_var<int>(&sets->misc.sm_speed, "misc", "sm_speed"));

		cvars.setup();

		for (auto cvar : cvars.all_cvars)
			floats.push_back(an_var<float>(&cvar.value, "console_variables", cvar.name));
	}

	bool write(string file)
	{
		if (file.empty()) return false;

		string path = "c:/lovemachine/configs/" + file + ".txt";

		if (_access(path.c_str(), 0) == -1)
		{
			auto handle = CreateFileA(path.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
			if (!handle || !CloseHandle(handle) || _access(path.c_str(), 0) == -1)
				return false;
		}

		for (auto var : bools)
			var.write(path);

		for (auto var : ints)
			var.write(path);

		for (auto var : floats)
			var.write(path);

		return true;
	}

	bool parse(string file)
	{
		if (file.empty()) return false;

		string path = "c:/lovemachine/configs/" + file + ".txt";

		if (_access(path.c_str(), 0) == -1)
		{
			auto handle = CreateFileA(path.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
			if (!handle || !CloseHandle(handle) || _access(path.c_str(), 0) == -1)
				return false;

			write(file);

			return false;
		}

		for (auto var : ints)
			var.parse(path);

		for (auto var : bools)
			var.parse(path);

		for (auto var : floats)
			var.parse(path);

		return true;
	}

	vector<char*> parse_configs()
	{
		vector<char*> configs;

		WIN32_FIND_DATAA find_data;

		HANDLE file_handle = FindFirstFileA("c:/lovemachine/configs/*.txt", &find_data);

		if (file_handle != INVALID_HANDLE_VALUE)
		{
			do
			{
				configs.push_back(find_data.cFileName);
			} while (FindNextFileA(file_handle, &find_data));

			FindClose(file_handle);
		}

		return configs;
	}
}