#pragma once
#include "includes.h"
#include "color.h"
#include "cvars.h"

enum bind_is : short
{
	bind_no_key = -1,
	bind_false = 0,
	bind_true = 1
};

struct bind_t
{
	short is()
	{
		if (key == 0)
			return bind_no_key;
		if ((hold == 1 && global::key[key]) || (hold == 0 && global::key_do[key]))
			return bind_true;
		return bind_false;
	}

	bool hold;
	int key;
};

class settings
{
public:
	settings()
	{
		//cvars.setup();
		//TODO : конфиги
		//CreateDirectory(L"C:/lovemachine", NULL);
		//CreateDirectory(L"C:/lovemachine/configs", NULL);
	}

	struct
	{
		bool opened = false, hovered = false, console = false, panic = false; // TODO : вместо panic лучше сделать анлоад чита
		int cont_hovered = -1;
		int x = 100, y = 100, cur_tab = 0;
	} menu;

	struct
	{
		bool opened = false, hovered = false;
		bool style[6] = { false, true, false, true, true, true };
		int x = 400, y = 100, mx = 200, my = 40;
	} info;

	struct
	{
		bool opened = false, hovered = false;
		int x = 670, y = 300, mx = 150, my = 30;
	} spec;

	struct
	{
		bool enabled = false;
		bool friends = false;
		bool knifebot = false;

		struct
		{
			bool enabled = true;
			bool hitbox[5] = { true, false, true, false, false };
			bool style[4] = { false, true, true, true };
			int ticks = 5;
		} backtrack;

		struct
		{
			bool hitbox[5] = { true, true, true, false, false };
			float fov = 4.f;//3;
			float smooth[2] = { 37.9f, 38.5f };//{ 49.f, 50.f };
			float rcs[2] = { 2.f, 2.f };
			float humanize[2] = { 1.6f, 1.2f };
			float kill_delay = 0.5f;
			float shot_delay = 0.15f;
		} aim;

		struct
		{
			bool _enabled = false; // конфликтует с legit.enabled
			bind_t bind = { false, 0 };
			bool hitbox[5] = { true, true, true, false, false };
			float delay = 0.07f;
		} trigger;
	} legit;

	struct
	{
		bool enabled = false;
		bool autowall = false;
		bool autoscope = false;
		bool autoshoot = false;
		bool autostop = false;
		bool silent = false;
		bool hitbox[5] = { true, false, true, false, false };
		float hitchance = 0.f;
	} rage;

	struct
	{
		bool enabled = false;
		bool friends = false;
		int chams = 1;
		bool chams_style[3] = { false, false, false };
		bool other_chams[3] = { false, false, false };
		bool other_style[5] = { true, true, false, false, false };
		color chams_t = color(150, 255, 13);
		color chams_ct = color(50, 255, 150);
		int crosshair = 2;
		bool esp_filter[6] = { false, false, false, false, false, false };
		bool esp_show[6] = { false, false, false, false, false, false };
		bool esp_bar[4] = { false, false, false, false };
		bool esp_check[2] = { false, false };
		color esp_t = color(210, 35, 16);
		color esp_ct = color(50, 90, 210);
		bool fade = true;
		bool defuser_only_if_need = false;
		//bool fancy_w2s = false;
		int ak47_skin = 0;
		int deagle_skin = 0;
		int knife_skin = 0;
		bool other_skins[2] = { false, false };
		bool remove[2] = { false, false };
		bool hitmarker = false;
		bool bomb_timer = false;
	} visuals;

	struct
	{
		bool autopistol = false;
		bool autojump = false;
		bool autostrafer = false;
		int aj_percent = 50;
		bool fl_spam_always = false;
		int fake_ping = 0;
		bool killshot = false;
		int lag_mode = 0;
		bind_t lag_spam = { false, 0 };
		int lag_factor = 0;
		bind_t fl_spam = { false, 0 };
		bind_t airstuck = { false, 0 };
		bind_t slowmotion = { false, 0 };
		int sm_speed = 4;
		bind_t record = { false, VK_NUMPAD7 };
		bind_t play = { false, VK_NUMPAD8 };
		int draw_mode = 0;
		float draw_time = 0.1f;
		color draw_color = color(0, 0, 120);
		bind_t draw_start = { false, VK_XBUTTON1 };
		bind_t draw_clear = { false, VK_XBUTTON2 };
	} misc;
};

settings* sets = new settings();