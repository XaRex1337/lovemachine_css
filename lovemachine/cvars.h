#pragma once
#include "includes.h"
#include "definitions.h"

enum cvar_type : int
{
	cvar_float,
	cvar_int,
	cvar_bool
};

enum cvar_ids : int
{
	// cvar_float
	//fov,
	vm_fov,
	kb_range,
	
	// cvar_int
	//ap_percent,
	//chams_t_r,
	//chams_t_g,
	//chams_t_b,
	//chams_ct_r,
	//chams_ct_g,
	//chams_ct_b,
	nightmode,

	// cvar_bool
	antismac,
	ragemode,

	// end
	cvar_max
};

struct ccvar
{
public:
	ccvar()
	{
		this->valid = false;
	}

	ccvar(const char* name, const char* comment, float def_value, float min = 0, float max = 100, int type = -1)
	{
		this->name = name;
		this->comment = comment != "" ? str(" is " + str(comment)) : " *no definition*";
		this->value = def_value;
		this->min = min;
		this->max = max;

		if (type != -1)
		{
			this->type = type;
		}
		else if (str(name).substr(0, 3) == "fl_")
		{
			this->type = cvar_float;
		}
		else if (str(name).substr(0, 4) == "int_")
		{
			this->type = cvar_int;
		}
		else if (str(name).substr(0, 2) == "b_")
		{
			this->type = cvar_bool;
			this->min = 0.f;
			this->max = 1.f;
		}

		this->valid = true;
	}
	
	string name, comment;
	float value, min, max;
	bool valid;
	int type = -1;
};

struct ccvars
{
public:
	deque<ccvar> all_cvars;

	void setup()
	{
		//all_cvars.push_back({ "fov", "main field of view", 90.f, 0.f, 179.f, cvar_float });
		all_cvars.push_back({ "vm_fov", "viewmodel fov", 90.f, 0.f, 179.f, cvar_float });
		all_cvars.push_back({ "kb_range", "knifebot distance to work", 55.f, 32.f, 68.f, cvar_float }); // 48

		//all_cvars.push_back({ "ap_percent", "percent of ap firing", 35.f, 0.f, 100.f, cvar_int });
		//all_cvars.push_back({ "chams_t_r", "color for terr. chams", 150.f, 0.f, 255.f, cvar_int });
		//all_cvars.push_back({ "chams_t_g", "color for terr. chams", 255.f, 0.f, 255.f, cvar_int }); // 25
		//all_cvars.push_back({ "chams_t_b", "color for terr. chams", 13.f, 0.f, 255.f, cvar_int });
		//all_cvars.push_back({ "chams_ct_r", "color for swat chams", 50.f, 0.f, 255.f, cvar_int });
		//all_cvars.push_back({ "chams_ct_g", "color for swat chams", 255.f, 0.f, 255.f, cvar_int }); // 90
		//all_cvars.push_back({ "chams_ct_b", "color for swat chams", 150.f, 0.f, 255.f, cvar_int });
		all_cvars.push_back({ "nightmode", "percent of wall darkness", 100.f, 0.f, 100.f, cvar_int });

		all_cvars.push_back({ "antismac", "smac proof functions", false, 0, 1, cvar_bool });
		all_cvars.push_back({ "ragemode", "rage functions", false, 0, 1, cvar_bool });
	}

	int find(string name)
	{
		if (all_cvars.empty()) return -1;

		for (int id = 0; id < cvar_max; id++)
		{
			//if (strstr(all_cvars.at(id).name.c_str(), name.c_str()) || strstr(name.c_str(), all_cvars.at(id).name.c_str()))
			if (name == all_cvars.at(id).name)
				return id;
		}

		return -1;
	}
};

ccvars cvars;

#define cvar(x) cvars.all_cvars.at(x)