#pragma once
#include "game shit.h"
#include "game def's.h"
#include "game classes.h"
#include "model shit.h"
#include "settings.h"
#include "backtrack.h"

//ofstream myfile;

using namespace legit::backtrack;

namespace models
{
	float base_alpha = 1.f;

	void end(imaterial* model_material, const DrawModelState_t& state, const ModelRenderInfo_t& p_info, matrix3x4_t* p_custom_bone_to_world)
	{
		_model_render->draw_model_execute(state, p_info, p_custom_bone_to_world);
		_model_render->forced_material_override(NULL);

		if (model_material && !model_material->is_error_material())
		{
			if (_render_view->get_blend() != base_alpha) _render_view->set_blend(base_alpha);

			model_material->set_materialvar_flag(MATERIAL_VAR_IGNOREZ, false);
			model_material->set_materialvar_flag(MATERIAL_VAR_WIREFRAME, false);
			model_material->set_materialvar_flag(MATERIAL_VAR_ENVMAPMODE, false);
		}
	}

	namespace chams
	{
		imaterial* materials[2][2][2];
		bool show_records = false;

		void apply_chams(imaterial* material, color p_color)
		{
			auto fcolor = p_color.divide();
			float model_color[4]; _render_view->get_color_modulation(model_color);
			float alpha = _render_view->get_blend();
			if (model_color != fcolor)
				_render_view->set_color_modulation(fcolor);
			if (fcolor[3] != alpha)
				_render_view->set_blend(fcolor[3] * base_alpha);

			if (material && !material->is_error_material())
				_model_render->forced_material_override(material);
		}

		namespace players
		{
			void run(imaterial* model_material, centity* entity, const DrawModelState_t& state, const ModelRenderInfo_t& p_info, matrix3x4_t* p_custom_bone_to_world)
			{
				color invis = entity->get_team() != global::local->get_team() ? color::disabled().with_alpha(sets->visuals.chams_style[2] ? 160 : 255) : color::background().with_alpha(sets->visuals.chams_style[2] ? 160 : 255);
				color vis = entity->get_team() == 2 ?
					sets->visuals.chams_t.with_alpha(sets->visuals.chams_style[2] ? 180 : 255) :
					sets->visuals.chams_ct.with_alpha(sets->visuals.chams_style[2] ? 180 : 255);
				/*205, 44, 13*//*70, 130, 180*/
// glow
				/*if (sets->visuals.chams_style[3])
				{
					apply_chams(materials[1][1][1][0], color(130, 200, 80));

					_model_render->draw_model_execute(state, p_info, p_custom_bone_to_world);
				}*/

				crecord record;
				cvector origin(0, 0, 0);

				if (legit::backtrack::draw && sets->legit.backtrack.style[0])
				{
					origin = entity->get_origin();
					record = records[p_info.entity_index][min(13, max(0, (global::cmd->tick_count - sets->legit.backtrack.ticks + 1) % sets->legit.backtrack.ticks))];
				}

				//invis
				if (sets->visuals.chams_style[0])
				{
					if (!model_material->get_materialvar_flag(MATERIAL_VAR_IGNOREZ))
					{
						model_material->set_materialvar_flag(MATERIAL_VAR_IGNOREZ, true);
					}

					apply_chams(materials[1][sets->visuals.chams == 2][sets->visuals.chams_style[1]], invis);

					_model_render->draw_model_execute(state, p_info, p_custom_bone_to_world);

					if (legit::backtrack::draw && sets->legit.backtrack.style[0])
					{
						if (sets->legit.backtrack.style[2] && best_record.id == p_info.entity_index && best_record.valid && best_record.have_matrix && best_record.hitbox_matrix && cvector(origin - best_record.origin).Length() > 1.f)
						{
							apply_chams(materials[1][sets->visuals.chams == 2][sets->visuals.chams_style[1]], color(15, 205, 15, 120));
							_model_render->draw_model_execute(state, p_info, best_record.hitbox_matrix);
						}
						else if (sets->legit.backtrack.style[3] && record.valid && record.have_matrix && record.hitbox_matrix && cvector(origin - record.origin).Length() > 1.f)
						{
							apply_chams(materials[1][sets->visuals.chams == 2][sets->visuals.chams_style[1]], color(220, 120, 25, 70));
							_model_render->draw_model_execute(state, p_info, record.hitbox_matrix);
						}
					}
				}

				if (legit::backtrack::draw && sets->legit.backtrack.style[0])
				{					
					if (sets->legit.backtrack.style[2] && best_record.is_valid(entity) && best_record.id == p_info.entity_index && best_record.have_matrix && best_record.hitbox_matrix && cvector(origin - best_record.origin).Length2D() > 1.f)
					{
						apply_chams(materials[0][sets->visuals.chams == 2][sets->visuals.chams_style[1]], color(30, 255, 30, 140));
						_model_render->draw_model_execute(state, p_info, best_record.hitbox_matrix);
					}

					if (record.is_valid(entity) && (record.id != best_record.id || record.tick != best_record.tick || !sets->legit.backtrack.style[2]) && sets->legit.backtrack.style[3] && record.have_matrix && record.hitbox_matrix && cvector(origin - record.origin).Length2D() > 1.f)
					{
						apply_chams(materials[0][sets->visuals.chams == 2][sets->visuals.chams_style[1]], color(255, 167, 25, 100));
						_model_render->draw_model_execute(state, p_info, record.hitbox_matrix);
					}
				}

				apply_chams(materials[0][sets->visuals.chams == 2][sets->visuals.chams_style[1]], vis);
			}
		}

		namespace other
		{
			void run(string name, imaterial* model_material, const DrawModelState_t& state, const ModelRenderInfo_t& p_info, matrix3x4_t* p_custom_bone_to_world)
			{
				if (strstr(name.c_str(), "models/weapons/") && ((sets->visuals.other_chams[0] && sets->visuals.other_chams[1]) ||
					((strstr(name.c_str(), "/v_") && sets->visuals.other_chams[0]) || (sets->visuals.other_chams[1] && strstr(name.c_str(), "/w_")))))
				{
					// v_ => в руке | w_ => в пространстве
					bool in_hand = strstr(name.c_str(), "/v_");

					if (sets->visuals.other_style[0] || sets->visuals.other_style[1])
					{
						// glow
						/*if (sets->visuals.other_style[5])
						{
							apply_chams(materials[1][1][1][0], color(255, 0, 0));

							_model_render->draw_model_execute(state, p_info, p_custom_bone_to_world);
						}*/

						/*if (!sets->visuals.other_style[0])
						{
							if (!model_material->get_materialvar_flag(MATERIAL_VAR_ENVMAPMODE) && sets->visuals.other_style[5])
								model_material->set_materialvar_flag(MATERIAL_VAR_ENVMAPMODE, true);
						}*/

						// invis
						if (sets->visuals.other_style[2] && !in_hand)
						{
							if (!model_material->get_materialvar_flag(MATERIAL_VAR_IGNOREZ))
							{
								model_material->set_materialvar_flag(MATERIAL_VAR_IGNOREZ, true);
							}

							apply_chams(sets->visuals.other_style[0] ? materials[1][0][sets->visuals.other_style[3]] : model_material,
								color(75 * (!sets->visuals.other_style[0] + 1), 0, 0, sets->visuals.other_style[4] ? 170 : 255));

							_model_render->draw_model_execute(state, p_info, p_custom_bone_to_world);
						}

						apply_chams(sets->visuals.other_style[0] ? materials[0][0][sets->visuals.other_style[3]] : model_material, color(25 *
							(!sets->visuals.other_style[0] + 1), 25 * (!sets->visuals.other_style[0] + 1), 112 * (!sets->visuals.other_style[0] + 1), sets->visuals.other_style[4] ? 170 : 255));
					}
					else if (sets->visuals.other_style[4])
					{
						apply_chams(model_material, color(255, 255, 255, 170));
					}
				}
				else if (sets->visuals.other_chams[2] && strstr(name.c_str(), "prop"))
				{
					if (_render_view->get_blend() != 0.51f) _render_view->set_blend(0.51f);
				}
			}
		}

		imaterial* create(bool ignorez, bool flat, bool add_shine)
		{
			static int id = 0;

			const char* baseType = (flat ? "UnlitGeneric" : "VertexLitGeneric");
			char material[512];

			static const char tmp[] = {
				"\"%s\"\
			\n{\
			\n\t\"$basetexture\" \"vgui/white_additive\"\
			\n\t\"$envmap\" \"%s\"\
			\n\t\"$normalmapalphaenvmapmask\" \"%i\"\
			\n\t\"$envmapcontrast\" \"%i\"\
			\n\t\"$model\" \"1\"\
			\n\t\"$flat\" \"1\"\
			\n\t\"$nocull\" \"0\"\
			\n\t\"$selfillum\" \"1\"\
			\n\t\"$halflambert\" \"1\"\
			\n\t\"$nofog\" \"0\"\
			\n\t\"$ignorez\" \"%i\"\
			\n\t\"$znearer\" \"0\"\
			\n\t\"$wireframe\" \"0\"\
			\n}\n"
			};

			sprintf_s(material, sizeof(material), tmp, baseType, add_shine ? "env_cubemap" : "", add_shine ? 1 : 0, add_shine ? 1 : 0, (ignorez) ? 1 : 0/*, (wireframe) ? 1 : 0*/);


			char name[512];
			sprintf_s(name, sizeof(name), "#textured_cham_material%i.vmt", id);
			id++;

			key_value* keyvalues = (key_value*)malloc(sizeof(key_value));
			key_values::singleton()->initialize(keyvalues, (char*)"UnlitGeneric");
			key_values::singleton()->load_from_buffer(keyvalues, name, material);

			imaterial* created_material = _mat_sys->create_material(name, (void*)keyvalues);
			if (!created_material) {
				return nullptr;
			} created_material->increment_reference_count();

			created_material->set_materialvar_flag(MATERIAL_VAR_IGNOREZ, ignorez);
			created_material->set_materialvar_flag(MATERIAL_VAR_FLAT, flat);
			created_material->set_materialvar_flag(MATERIAL_VAR_WIREFRAME, false);

			return created_material;
		}
	}

	namespace skins
	{
		enum e_material
		{
			v_ak47_adidas,
			w_ak47_adidas,
			ak47_bloodsport,
			ak47_neon_rider,
			v_kitty_deagle,
			w_kitty_deagle,
			survival_bowie,
			v_karam,
			w_karam,
			bomb,
			flashbang,
			fraggrenade,
			smokegrenade,
			material_max
		};

		const char* s_material[material_max]
		{
			"vgui/ak47/v_adidas",
			"vgui/ak47/w_adidas",
			"vgui/ak47/bloodsport",
			"vgui/ak47/neon_rider",
			"vgui/deagle/v_kitty",
			"vgui/deagle/w_kitty",
			"vgui/knife/survival_bowie",
			"vgui/knife/v_karam",
			"vgui/knife/w_karam",
			"vgui/other/C4",
			"vgui/other/flashbang",
			"vgui/other/fraggrenade",
			"vgui/other/smokegrenade"
		};

		imaterial* materials[material_max];

		void run(string name)
		{
			imaterial* p_material = nullptr;
			
			if (!strstr(name.c_str(), "models/weapons/")) return;

			if (strstr(name.c_str(), "ak47"))
			{
				switch (sets->visuals.ak47_skin)
				{
				case 1: p_material = strstr(name.c_str(), "/v_") ? materials[v_ak47_adidas] : materials[w_ak47_adidas]; break;
				case 2: p_material = materials[ak47_bloodsport]; break;
				case 3: p_material = materials[ak47_neon_rider]; break;
				}
			}
			else if (strstr(name.c_str(), "deagle"))
			{
				switch (sets->visuals.deagle_skin)
				{
				case 1: p_material = strstr(name.c_str(), "/v_") ? materials[v_kitty_deagle] : materials[w_kitty_deagle]; break;
				}
			}
			else if (strstr(name.c_str(), "knife"))
			{
				switch (sets->visuals.knife_skin)
				{
				case 1: p_material = materials[survival_bowie]; break;
				case 2: p_material = strstr(name.c_str(), "/v_") ? materials[v_karam] : materials[w_karam]; break;
				}
			}
			else if (strstr(name.c_str(), "c4") && sets->visuals.other_skins[0])
			{
				p_material = materials[bomb];
			}
			else if (sets->visuals.other_skins[1])
			{
				if (strstr(name.c_str(), "flash")) p_material = materials[flashbang];
				else if (strstr(name.c_str(), "smoke")) p_material = materials[smokegrenade];
				else if (strstr(name.c_str(), "frag")) p_material = materials[fraggrenade];
			}

			if (p_material) _model_render->forced_material_override(p_material);
		}

		imaterial* create(string path)
		{
			static int id = 0;

			static const char tmp[] = {
				"\"VertexLitGeneric\"\
			\n{\
			\n\t\"$basetexture\" \"%s\"\
			\n}\n"
			};

			char material[512];
			sprintf_s(material, sizeof(material), tmp, path.c_str());

			char name[512];
			sprintf_s(name, sizeof(name), "#skin_material%i.vmt", id);
			id++;

			key_value* keyvalues = (key_value*)malloc(sizeof(key_value));
			key_values::singleton()->initialize(keyvalues, (char*)"UnlitGeneric");
			key_values::singleton()->load_from_buffer(keyvalues, name, material);

			imaterial* created_material = _mat_sys->create_material(name, (void*)keyvalues);
			if (!created_material) {
				return nullptr;
			} created_material->increment_reference_count();

			return created_material;
		}
	}

	void on_inject()
	{
		for (int ignorez = 0; ignorez < 2; ignorez++)
			for (int flat = 0; flat < 2; flat++)
				for (int glow = 0; glow < 2; glow++)
					for (int add_shine = 0; add_shine < 2; add_shine++)
						chams::materials[ignorez][flat][add_shine] = chams::create(ignorez, flat, add_shine);

		for (int id = 0; id < skins::material_max; id++)
			skins::materials[id] = skins::create(skins::s_material[id]);
	}

	void run(const DrawModelState_t& state, const ModelRenderInfo_t& p_info, matrix3x4_t* p_custom_bone_to_world)
	{
		if (sets->menu.panic || !sets->visuals.enabled || !_engine->is_connected() || !_engine->in_game() || !global::cmd || global::cmd->command_number == 0 || !global::local)
		{
			end(nullptr, state, p_info, p_custom_bone_to_world);
			return;
		}

		centity* entity = _ent_list->get_centity(p_info.entity_index);

		if (!entity || entity->is_dormant())
		{
			end(nullptr, state, p_info, p_custom_bone_to_world);
			return;
		}

		static imaterial* model_material[32]; _model_info->get_model_materials(p_info.pModel, 1, model_material);
		base_alpha = _render_view->get_blend();
		auto name = _model_info->get_model_name(p_info.pModel);
		const auto is_player = p_info.entity_index >= 1 && strstr(name, "player");
		//if (is_player && p_info.entity_index < 64)
		//	server::players[p_info.entity_index].drawable = true;
		
		if (is_player && (!entity->valid() || (!sets->visuals.friends && entity->get_team() == global::local->get_team())))
		{
			end(nullptr, state, p_info, p_custom_bone_to_world);
			return;
		}

		if (is_player && sets->visuals.chams > 0)
		{
			chams::players::run(model_material[0], entity, state, p_info, p_custom_bone_to_world);
		}

		if (!is_player && (sets->visuals.other_chams[0] || sets->visuals.other_chams[1] || sets->visuals.other_chams[2]))
		{
			chams::other::run(name, model_material[0], state, p_info, p_custom_bone_to_world);
		}

		if (!is_player && (sets->visuals.ak47_skin > 0 || sets->visuals.deagle_skin > 0 || sets->visuals.other_skins[0] || sets->visuals.other_skins[1]))
		{
			skins::run(name);
		}

		end(model_material[0], state, p_info, p_custom_bone_to_world);
	}
}