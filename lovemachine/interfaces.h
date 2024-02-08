#pragma once
#include "includes.h"
#include "definitions.h"
#include "memory.h"
#include "dt_recv.h"
#include "vector.h"
#include "game def's.h"
#include "model shit.h"

class ivpanel
{
public:

	const char* get_name(unsigned int vguiPanel)
	{
		typedef const char* (__thiscall* get_name_fn)(void*, unsigned int);
		return vfunc< get_name_fn >(this, 36)(this, vguiPanel);
	}

	void painttraverse(unsigned int vguiPanel, bool forceRepaint, bool allowForce = true)
	{
		typedef void(__thiscall* painttraverse_fn)(void*, unsigned int, bool, bool);
		vfunc< painttraverse_fn >(this, 41)(this, vguiPanel, forceRepaint, allowForce);
	}

	unsigned int idkwhatisthis(int input)
	{
		typedef unsigned int(__thiscall* idkwhatisthis_fn)(void*, int);
		return vfunc< idkwhatisthis_fn >(this, 1)(this, input);
	}
};



class isurface
{
public:
	void set_color(int r, int g, int b, int a)
	{
		typedef void(__thiscall* set_color_fn)(void*, int, int, int, int);
		vfunc< set_color_fn >(this, 11)(this, r, g, b, a);
	}

	void filled_rect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* filled_rect_fn)(void*, int, int, int, int);
		vfunc< filled_rect_fn >(this, 12)(this, x0, y0, x1, y1);
	}

	void outlined_rect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* outlined_rect_fn)(void*, int, int, int, int);
		vfunc< outlined_rect_fn >(this, 14)(this, x0, y0, x1, y1);
	}

	void line(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* line_fn)(void*, int, int, int, int);
		vfunc< line_fn >(this, 15)(this, x0, y0, x1, y1);
	}

	void polyline(int* px, int* py, int num)
	{
		typedef void(__thiscall* polyline_fn)(void*, int*, int*, int);
		vfunc< polyline_fn >(this, 16)(this, px, py, num);
	}

	void set_font(unsigned long font)
	{
		typedef void(__thiscall* set_font_fn)(void*, unsigned long);
		vfunc< set_font_fn >(this, 17)(this, font);
	}

	void set_text_color(int r, int g, int b, int a)
	{
		typedef void(__thiscall* set_text_color_fn)(void*, int, int, int, int);
		vfunc< set_text_color_fn >(this, 19)(this, r, g, b, a);
	}

	void set_text_pos(int x, int y)
	{
		typedef void(__thiscall* set_text_pos_fn)(void*, int, int);
		vfunc< set_text_pos_fn >(this, 20)(this, x, y);
	}
	
	void print_text(const wchar_t* text, int length, int draw_type = 0)
	{
		typedef void(__thiscall* print_text_fn)(void*, const wchar_t*, int, int);
		vfunc< print_text_fn >(this, 22)(this, text, length, draw_type);
	}

	unsigned long create_font()
	{
		typedef unsigned long(__thiscall* create_font_fn)(void*);
		return vfunc< create_font_fn >(this, 66)(this);
	}

	bool set_font_glyph_set(unsigned long font, const char* windows_font_name, int tall, int weight, int blur, int scanlines, int flags)
	{
		typedef bool(__thiscall* set_font_glyph_set_fn)(void*, unsigned long, const char*, int, int, int, int, int);
		return vfunc< set_font_glyph_set_fn >(this, 67)(this, font, windows_font_name, tall, weight, blur, scanlines, flags);
	}

	void get_text_size(unsigned long font, const wchar_t* text, int& wide, int& tall)
	{
		typedef void(__thiscall* get_text_size_fn)(void*, unsigned long, const wchar_t*, int&, int&);
		vfunc< get_text_size_fn >(this, 75)(this, font, text, wide, tall);
	}

	void set_cursor(unsigned long cursor)
	{
		typedef void(__thiscall* set_cursor_fn)(void*, unsigned long);
		vfunc< set_cursor_fn >(this, 58)(this, cursor);
	}

	void unlock_cursor()
	{
		typedef void(__thiscall* unlock_cursor_fn)(void*);
		vfunc< unlock_cursor_fn >(this, 64)(this);
	}
};

class centity;

class icliententitylist
{
public:
	virtual void f1() = 0;
	virtual void f2() = 0;
	virtual void f3() = 0;

	virtual centity* get_centity(int iIndex) = 0;
	virtual centity* get_centity_from_handle(DWORD hHandle) = 0;
	virtual int number_of_ents(bool bIncludeNonNetworkable) = 0;
	virtual int get_highest_entity_index() = 0;
};

typedef struct
{
	char			name[32]; // scoreboard information
	int				user_id; // local server user ID, unique while server is running
	char			guid[33]; // global unique player identifer
	__int32			friends_id; // friends identification number
	char			friends_name[32]; // friends name
	bool			fakeplayer; // true, if player is a bot controlled by game.dll
	bool			ishltv; // true if player is the HLTV proxy
	unsigned int	custom_files[4]; // (CRC32_t) custom files CRC for this player
	unsigned char	files_downloaded; // this counter increases each time the server downloaded a new file
	char pad[200];
} player_info_t;

class ivengineclient
{
public:
	void get_screen_size(int& width, int& height)
	{
		typedef void(__thiscall* get_screen_size_fn)(void*, int&, int&);
		vfunc< get_screen_size_fn >(this, 5)(this, width, height);
	}

	bool get_playerinfo(int ent_num, player_info_t* pinfo)
	{
		typedef bool(__thiscall* get_playerinfo_fn)(void*, int, player_info_t*);
		return vfunc< get_playerinfo_fn >(this, 8)(this, ent_num, pinfo);
	}

	int get_player_for_userid(int userid)
	{
		typedef int(__thiscall* get_player_for_userid_fn)(void*, int);
		return vfunc< get_player_for_userid_fn >(this, 9)(this, userid);
	}

	int get_local_id()
	{
		typedef int(__thiscall* get_local_id_fn)(void*);
		return vfunc< get_local_id_fn >(this, 12)(this);
	}

	void get_viewangles(qangle& va)
	{
		typedef int(__thiscall* get_viewangles_fn)(void*, qangle&);
		vfunc< get_viewangles_fn >(this, 19)(this, va);
	}

	void set_viewangles(qangle& va)
	{
		typedef int(__thiscall* set_viewangles_fn)(void*, qangle&);
		vfunc< set_viewangles_fn >(this, 20)(this, va);
	}

	int get_max_clients()
	{
		typedef int(__thiscall* get_max_clients_fn)(void*);
		return vfunc< get_max_clients_fn >(this, 21)(this);
	}

	bool in_game()
	{
		typedef bool(__thiscall* in_game_fn)(void*);
		return vfunc< in_game_fn >(this, 26)(this);
	}

	bool is_connected()
	{
		typedef bool(__thiscall* is_connected_fn)(void*);
		return vfunc< is_connected_fn >(this, 27)(this);
	}

	const matrix4x4& w2s_matrix()//const matrix3x4& w2s_matrix()
	{
		typedef const matrix4x4& (__thiscall* w2s_matrix_fn)(void*);
		return vfunc< w2s_matrix_fn >(this, 36)(this); //36 - 37

		//typedef const matrix3x4&(__thiscall* w2s_matrix_fn)(void*);
		//return vfunc< w2s_matrix_fn >(this, 36)(this); //36 - 37
	}

	bool is_screenshoting() // ïî÷åìó-òî âñåãäà true (ñêîðåå âñåãî èç-çà òîãî, ÷òî ýòî íîíñòèì)
	{ // upd. - õç êàêîé èíäåêñ, ìá ýòîãî äàæå íåò
		typedef bool(__thiscall* is_screenshoting_fn)(void*);
		return vfunc< is_screenshoting_fn >(this, 77)(this); // 91 //114 - 12 = 102 //91 - 12 = 79
	}

	void clientcmd_unrestricted(const char* szCmdString)
	{
		typedef void(__thiscall* clientcmd_unrestricted_fn)(void*, const char*);
		vfunc< clientcmd_unrestricted_fn >(this, 106)(this, szCmdString);
	}
};

class clientclass
{
public:
	CreateClientClassFn create_fn;
	CreateEventFn		createvent_fn;	// Only called for event objects.
	const char*			name;
	RecvTable*			recv_table;
	clientclass*		next;
	int					class_id;	// Managed by the engine.
};

//-----------------------------------------------------------------------------
// Flags passed in with view setup
//-----------------------------------------------------------------------------
enum ClearFlags_t
{
	VIEW_CLEAR_COLOR = 0x1,
	VIEW_CLEAR_DEPTH = 0x2,
	VIEW_CLEAR_FULL_TARGET = 0x4,
	VIEW_NO_DRAW = 0x8,
	VIEW_CLEAR_OBEY_STENCIL = 0x10,
};


//-----------------------------------------------------------------------------
// Purpose: Renderer setup data.  
//-----------------------------------------------------------------------------
class cviewsetup
{
public:
	cviewsetup()
	{
		m_flAspectRatio = 0.0f;
		m_bRenderToSubrectOfLargerScreen = false;
		m_bDoBloomAndToneMapping = true;
		m_bOffCenter = false;
		m_bCacheFullSceneState = false;
		//		m_bUseExplicitViewVector = false;
	}

	// shared by 2D & 3D views

		// left side of view window
	int			x;
	// top side of view window
	int			y;
	// width of view window
	int			width;
	// height of view window
	int			height;

	// the rest are only used by 3D views

		// Orthographic projection?
	bool		m_bOrtho;
	// View-space rectangle for ortho projection.
	float		m_OrthoLeft;
	float		m_OrthoTop;
	float		m_OrthoRight;
	float		m_OrthoBottom;

	// horizontal FOV in degrees
	float		fov;
	// horizontal FOV in degrees for in-view model
	float		fovViewmodel;

	// 3D origin of camera
	Vector		origin;

	// heading of camera (pitch, yaw, roll)
	qangle		angles;
	// local Z coordinate of near plane of camera
	float		zNear;
	// local Z coordinate of far plane of camera
	float		zFar;

	// local Z coordinate of near plane of camera ( when rendering view model )
	float		zNearViewmodel;
	// local Z coordinate of far plane of camera ( when rendering view model )
	float		zFarViewmodel;

	// set to true if this is to draw into a subrect of the larger screen
	// this really is a hack, but no more than the rest of the way this class is used
	bool		m_bRenderToSubrectOfLargerScreen;

	// The aspect ratio to use for computing the perspective projection matrix
	// (0.0f means use the viewport)
	float		m_flAspectRatio;

	// Controls for off-center projection (needed for poster rendering)
	bool		m_bOffCenter;
	float		m_flOffCenterTop;
	float		m_flOffCenterBottom;
	float		m_flOffCenterLeft;
	float		m_flOffCenterRight;

	// Control that the SFM needs to tell the engine not to do certain post-processing steps
	bool		m_bDoBloomAndToneMapping;

	// Cached mode for certain full-scene per-frame varying state such as sun entity coverage
	bool		m_bCacheFullSceneState;
};

#define WRAP_READ( bc ) 																									  \
class bf_read : public bc																									  \
{																															  \
public:																														  \
    FORCEINLINE bf_read( void ) : bc(  )																								  \
	{																														  \
	}																														  \
																															  \
	FORCEINLINE bf_read( const void *pData, int nBytes, int nBits = -1 ) : bc( pData, nBytes, nBits )									  \
	{																														  \
	}																														  \
																															  \
	FORCEINLINE bf_read( const char *pDebugName, const void *pData, int nBytes, int nBits = -1 ) : bc( pDebugName, pData, nBytes, nBits ) \
	{																														  \
	}																														  \
};

#define WRAP_WRITE( bc )																									   \
class bf_write : public bc																									   \
{																															   \
public:																														   \
	FORCEINLINE bf_write(void) : bc()																									   \
	{																														   \
	}																														   \
	FORCEINLINE bf_write( void *pData, int nBytes, int nMaxBits = -1 ) : bc( pData, nBytes, nMaxBits )									   \
	{																														   \
	}																														   \
																															   \
	FORCEINLINE bf_write( const char *pDebugName, void *pData, int nBytes, int nMaxBits = -1 ) : bc( pDebugName, pData, nBytes, nMaxBits ) \
	{																														   \
	}																														   \
};

class ibaseclientdll
{
public:
	clientclass* get_all_classes()
	{
		typedef clientclass*(__thiscall* get_all_classes_fn)(void*);
		return vfunc< get_all_classes_fn >(this, 8)(this);
	}

	//23
	void create_move(int sequence_number,			// sequence_number of this cmd
					float input_sample_frametime,	// Frametime for mouse input sampling
					bool active)
	{
		typedef void(__thiscall* create_move_fn)(void*, int, float, bool);
		vfunc< create_move_fn >(this, 21)(this, sequence_number, input_sample_frametime, active);
	}

	//dword => bf_write
	bool write_usercmd_delta_to_buf(dword* buf, int from, int to, bool isnewcommand)
	{
		typedef bool(__thiscall* write_usercmd_delta_from_buf_fn)(void*, dword*, int, int, bool);
		return vfunc< write_usercmd_delta_from_buf_fn >(this, 23)(this, buf, from, to, isnewcommand);
	}

	void render_view(const cviewsetup& view, int nClearFlags, int whatToDraw)
	{
		typedef void(__thiscall* render_view_fn)(void*, const cviewsetup&, int, int);
		vfunc< render_view_fn >(this, 27)(this, view, nClearFlags, whatToDraw);
	}
};

class ivdebugoverlay
{
public:
	int screen_position(const Vector& point, Vector& screen)
	{
		typedef int(__thiscall* screen_position_fn)(void*, const Vector&, Vector&);
		return vfunc< screen_position_fn >(this, 10)(this, point, screen);
	}
};

class cusercmd;

class cinput
{
public:
	cusercmd* get_usercmd(int sequence_number)
	{
		typedef cusercmd*(__thiscall* get_usercmd_fn)(void*, int);
		return vfunc< get_usercmd_fn >(this, 8)(this, sequence_number);
	}
};

class imaterial_var;
struct mat_property_types_t;
class key_values;

// (c) catalindragan22
class imaterial
{
public:
	virtual const char* get_name() const = 0;
	virtual const char* get_texture_group_name() const = 0;
	virtual DWORD get_preview_image_properies(int* width, int* height, DWORD* imageFormat, bool* isTranslucent) const = 0;
	virtual DWORD get_preview_image(unsigned char* data, int width, int height, DWORD imageFormat) const = 0;
	virtual int get_mapping_width() = 0;
	virtual int get_mapping_height() = 0;
	virtual int get_num_animation_frames() = 0;
	virtual bool in_material_page(void) = 0;
	virtual void get_material_offset(float* pOffset) = 0;
	virtual void get_material_scale(float* pScale) = 0;
	virtual imaterial* get_material_page(void) = 0;
	virtual imaterial_var* find_var(const char* varName, bool* found, bool complain = true) = 0;
	virtual void increment_reference_count(void) = 0;
	virtual void decrement_reference_count(void) = 0;
	virtual int get_enumeration_id(void) const = 0;
	virtual void get_low_res_colour_sample(float s, float t, float* color) const = 0;
	virtual void recompute_state_snapshot() = 0;
	virtual bool is_translucent() = 0;
	virtual bool is_alpha_tested() = 0;
	virtual bool is_vertex_lit() = 0;
	virtual uint64_t get_vertex_format() const = 0;
	virtual bool has_proxy(void) const = 0;
	virtual bool uses_envcube_map(void) = 0;
	virtual bool needs_tangent_space(void) = 0;
	virtual bool needs_power_of_two_frame_buffer_texture(bool bCheckSpecificToThisFrame = true) = 0;
	virtual bool needs_full_frame_buffer_texture(bool bCheckSpecificToThisFrame = true) = 0;
	virtual bool needs_software_skinning(void) = 0;
	virtual void alpha_modulate(float alpha) = 0;
	virtual void colour_modulate(float r, float g, float b) = 0;
	virtual void set_materialvar_flag(mat_var_flags flag, bool on) = 0;
	virtual bool get_materialvar_flag(mat_var_flags flag) = 0;
	virtual void get_reflectivity(Vector& reflect) = 0;
	virtual bool get_property_flag(mat_property_types_t type) = 0;
	virtual bool is_two_sided() = 0;
	virtual void set_shader(const char* pShaderName) = 0;
	virtual int get_num_passes(void) = 0;
	virtual int get_texture_memory_bytes(void) = 0;
	virtual void refresh() = 0;
	virtual bool needs_lightmap_blend_alpha(void) = 0;
	virtual bool needs_software_lighting(void) = 0;
	virtual int shader_param_count() const = 0;
	virtual imaterial_var** get_shader_params(void) = 0;
	virtual bool is_error_material() const = 0;
	virtual void unused() = 0;
	virtual float get_alpha_modulation() = 0;
	virtual void get_colour_modulation(float* r, float* g, float* b) = 0;
	virtual bool is_translucent_under_modulation(float fAlphaModulation = 1.0f) const = 0;
	virtual imaterial_var* find_var_fast(char const* pVarName, unsigned int* pToken) = 0;
	virtual void set_shader_and_params(void* pKeyValues) = 0;
	virtual const char* get_shader_name() const = 0;
	virtual void delete_if_unreferenced() = 0;
	virtual bool is_sprite_card() = 0;
	virtual void call_bind_proxy(void* proxyData) = 0;
	virtual void refresh_preserving_materialvars() = 0;
	virtual bool was_reloaded_from_whitelist() = 0;
	virtual bool set_temp_excluded(bool bSet, int nExcludedDimensionLimit) = 0;
	virtual int get_reference_count() const = 0;
};

class imaterialsystem
{
public:
	imaterial* create_material(const char* pMaterialName, void* pVMTKeyValues)
	{
		typedef imaterial*(__thiscall* create_material_fn)(void*, const char*, void*);
		return vfunc< create_material_fn >(this, 70)(this, pMaterialName, pVMTKeyValues);
	}

	imaterial* find_material(char const* pMaterialName, const char* pTextureGroupName, bool complain = true, const char* pComplainPrefix = NULL)
	{
		typedef imaterial* (__thiscall* find_material_fn)(void*, char const*, const char*, bool, const char*);
		return vfunc< find_material_fn >(this, 71)(this, pMaterialName, pTextureGroupName, complain, pComplainPrefix);
	}

	materialhandle_t first_material()
	{
		typedef materialhandle_t(__thiscall* first_material_fn)(void*);
		return vfunc< first_material_fn >(this, 73)(this);
	}

	materialhandle_t next_material(materialhandle_t handle)
	{
		typedef materialhandle_t(__thiscall* next_material_fn)(void*, materialhandle_t);
		return vfunc< next_material_fn >(this, 74)(this, handle);
	}

	materialhandle_t invalid_material()
	{
		typedef materialhandle_t(__thiscall* invalid_material_fn)(void*);
		return vfunc< invalid_material_fn >(this, 75)(this);
	}

	imaterial* get_material(materialhandle_t handle)
	{
		typedef imaterial*(__thiscall* get_material_fn)(void*, materialhandle_t);
		return vfunc< get_material_fn >(this, 76)(this, handle);
	}
};

class ivmodelrender
{
public:
	/*int		draw_model(int flags,
		iclientrenderable* pRenderable,
		ModelInstanceHandle_t instance,
		int entity_index,
		const model_t* model,
		Vector const& origin,
		qangle const& angles,
		int skin,
		int body,
		int hitboxset,
		const matrix3x4_t* modelToWorld = NULL,
		const matrix3x4_t* pLightingOffset = NULL)
	{
		typedef int(__thiscall* draw_model_fn)(void*, int, iclientrenderable*, ModelInstanceHandle_t, int, const model_t*, Vector const&, qangle const&, int, int, int, const matrix3x4_t*, const matrix3x4_t*);
		return vfunc< draw_model_fn >(this, 1)(this, flags, pRenderable, instance, entity_index, model, origin, angles, skin, body, hitboxset, modelToWorld, pLightingOffset);
	}

	void forced_material_override(imaterial* new_material, OverrideType_t override_type = OVERRIDE_NORMAL)
	{
		typedef void(__thiscall* forced_material_override_fn)(void*, imaterial*, OverrideType_t);
		vfunc< forced_material_override_fn >(this, 2)(this, new_material, override_type);
	}

	void draw_model_execute(const DrawModelState_t& state, const ModelRenderInfo_t& p_info, matrix3x4_t* p_custom_bone_to_world = NULL)
	{
		typedef void(__thiscall* create_material_fn)(void*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);
		vfunc< create_material_fn >(this, 19)(this, state, p_info, p_custom_bone_to_world); // 19 ???
	}*/

	virtual int		vfunc00() = 0;
	virtual void	forced_material_override(imaterial* newMaterial, OverrideType_t nOverrideType = OVERRIDE_NORMAL) = 0;
	virtual bool	vfunc02() = 0;
	virtual int		vfunc03() = 0;
	virtual int		vfunc04() = 0;
	virtual int		vfunc05() = 0;
	virtual int		vfunc06() = 0;
	virtual int		vfunc07() = 0;
	virtual int		vfunc08() = 0;
	virtual int		vfunc09() = 0;
	virtual int		vfunc10() = 0;
	virtual int		vfunc11() = 0;
	virtual int		vfunc12() = 0;
	virtual int		vfunc13() = 0;
	virtual int		vfunc14() = 0;
	virtual int		vfunc15() = 0;
	virtual int		vfunc16() = 0;
	virtual int		vfunc17() = 0;
	virtual int		vfunc18() = 0;
	virtual void	draw_model_execute(const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld = NULL) = 0;
};

class ivmodelinfo
{
public:
	const char* get_model_name(const model_t* model)
	{
		typedef const char*(__thiscall* get_model_name_fn)(void*, const model_t*);
		return vfunc< get_model_name_fn >(this, 3)(this, model);
	}

	void get_model_materials(const model_t* model, int count, imaterial** ppMaterial)
	{
		typedef void(__thiscall* get_model_materials_fn)(void*, const model_t*, int, imaterial**);
		vfunc< get_model_materials_fn >(this, 16)(this, model, count, ppMaterial);
	}

	model_t* find_or_load_model(const char* name)
	{
		typedef model_t*(__thiscall* find_or_load_model_fn)(void*, const char*);
		return vfunc< find_or_load_model_fn >(this, 39)(this, name);
	}
};

class ivrenderview
{
public:
	void set_blend(float blend)
	{
		typedef void(__thiscall* set_blend_fn)(void*, float);
		vfunc< set_blend_fn >(this, 4)(this, blend);
	}

	float get_blend()
	{
		typedef float(__thiscall* get_blend_fn)(void*);
		return vfunc< get_blend_fn >(this, 5)(this);
	}

	void set_color_modulation(float const* blend)
	{
		typedef void(__thiscall* set_blend_fn)(void*, float const*);
		vfunc< set_blend_fn >(this, 6)(this, blend);
	}

	void get_color_modulation(float* blend)
	{
		typedef float(__thiscall* get_blend_fn)(void*, float*);
		vfunc< get_blend_fn >(this, 7)(this, blend);
	}
};

class CSaveRestoreData;

class cglobalvars
{
public:
	// Absolute time (per frame still - Use Plat_FloatTime() for a high precision real time 
	//  perf clock, but not that it doesn't obey host_timescale/host_framerate)
	float			realtime;
	// Absolute frame counter
	int				framecount;
	// Non-paused frametime
	float			absoluteframetime;

	// Current time 
	//
	// On the client, this (along with tickcount) takes a different meaning based on what
	// piece of code you're in:
	// 
	//   - While receiving network packets (like in PreDataUpdate/PostDataUpdate and proxies),
	//     this is set to the SERVER TICKCOUNT for that packet. There is no interval between
	//     the server ticks.
	//     [server_current_Tick * tick_interval]
	//
	//   - While rendering, this is the exact client clock 
	//     [client_current_tick * tick_interval + interpolation_amount]
	//
	//   - During prediction, this is based on the client's current tick:
	//     [client_current_tick * tick_interval]
	float			curtime;

	// Time spent on last server or client frame (has nothing to do with think intervals)
	float			frametime;
	// current maxplayers setting
	int				maxClients;

	// Simulation ticks
	int				tickcount;

	// Simulation tick interval
	float			interval_per_tick;

	// interpolation amount ( client-only ) based on fraction of next tick which has elapsed
	float			interpolation_amount;
	int				simTicksThisFrame;

	int				network_protocol;

	// current saverestore data
	CSaveRestoreData* pSaveData;

private:
	// Set to true in client code.
	bool			m_bClient;

	// 100 (i.e., tickcount is rounded down to this base and then the "delta" from this base is networked
	int				nTimestampNetworkingBase;
	// 32 (entindex() % nTimestampRandomizeWindow ) is subtracted from gpGlobals->tickcount to set the networking basis, prevents
	//  all of the entities from forcing a new PackedEntity on the same tick (i.e., prevents them from getting lockstepped on this)
	int				nTimestampRandomizeWindow;
};

class iplayerinfomanager
{
public:
	virtual void** somefunc(void** idk) = 0;//virtual IPlayerInfo* GetPlayerInfo(edict_t* pEdict) = 0;
	virtual cglobalvars* get_globalvars() = 0;
};

class cbaseviewmodel
{
public:
	//4
	virtual void					UpdateOnRemove(void);

	// Weapon client handling
	virtual void			SendViewModelMatchingSequence(int sequence);
	virtual void			SetWeaponModel(const char* pszModelname, void** weapon);

	virtual void			CalcViewModelLag(Vector& origin, qangle& angles, qangle& original_angles);
	virtual void			CalcViewModelView(centity* owner, const Vector& eyePosition, const qangle& eyeAngles);
};

class iclientmode
{
public:
	void override_view(cviewsetup* p_setup)
	{
		typedef void(__thiscall* override_view_fn)(void*, cviewsetup*);
		vfunc< override_view_fn >(this, 16)(this, p_setup);
	}

	float get_vm_fov()
	{
		typedef float(__thiscall* get_vm_fov_fn)(void*);
		return vfunc< get_vm_fov_fn >(this, 32)(this);
	}
};

class itracefilter;

class ienginetrace
{
public:
	int get_point_contents(const Vector& vecAbsPosition, void** ppEntity)
	{
		typedef int(__thiscall* get_point_contents_fn)(void*, const Vector&, void**);
		return vfunc< get_point_contents_fn >(this, 0)(this, vecAbsPosition, ppEntity);
	}

	void trace_ray(const ray_t& ray, unsigned int mask, itracefilter* tracefilter, trace_t* trace)
	{
		typedef void(__thiscall* trace_ray_fn)(void*, const ray_t&, unsigned int, itracefilter*, trace_t*);
		vfunc< trace_ray_fn >(this, 4)(this, ray, mask, tracefilter, trace);
	}
};

class irecipientfilter;

class ienginesound
{
public:
	void emit_sound(irecipientfilter& filter, int iEntIndex, int iChannel, const char* pSample,
		float flVolume, float flAttenuation, int iFlags = 0, int iPitch = 100/*PITCH_NORM*/, int iSpecialDSP = 0,
		const Vector* pOrigin = NULL, const Vector* pDirection = NULL, void* pUtlVecOrigins = NULL, bool bUpdatePositions = true, float soundtime = 0.0f, int speakerentity = -1)
	{
		typedef void(__thiscall* emit_sound_fn)(void*, irecipientfilter&, int, int, const char*, float, float, int, int, int, const Vector*, const Vector*, void*, bool, float, int);
		vfunc< emit_sound_fn >(this, 4)(this, filter, iEntIndex, iChannel, pSample, flVolume, flAttenuation, iFlags, iPitch, iSpecialDSP, pOrigin, pDirection, pUtlVecOrigins, bUpdatePositions, soundtime, speakerentity);
	}
};

class ibaseinterface
{
public:
	virtual	~ibaseinterface(void) {};
};

class igameeventmanager : public ibaseinterface
{
public:
	virtual	~igameeventmanager(void) {};

	// load game event descriptions from a file eg "resource\gameevents.res"
	virtual int load_events_from_file(const char* filename) = 0;

	// removes all and anything
	virtual void reset() = 0;

	// adds a listener for a particular event
	virtual bool add_listener(void* listener, const char* name, bool bServerSide) = 0;

	// returns true if this listener is listens to given event
	virtual bool find_listener(void* listener, const char* name) = 0;

	// removes a listener 
	virtual void remove_listener(void* listener) = 0;

	// create an event by name, but doesn't fire it. returns NULL is event is not
	// known or no listener is registered for it. bForce forces the creation even if no listener is active
	virtual igameevent* create_event(const char* name, bool bForce = false) = 0;

	// fires a server event created earlier, if bDontBroadcast is set, event is not send to clients
	virtual bool fire_event(igameevent* event, bool bDontBroadcast = false) = 0;

	// fires an event for the local client only, should be used only by client code
	virtual bool fire_event_client_side(igameevent* event) = 0;

	// create a new copy of this event, must be free later
	virtual igameevent* duplicate_event(igameevent* event) = 0;

	// if an event was created but not fired for some reason, it has to bee freed, same UnserializeEvent
	virtual void free_event(igameevent* event) = 0;

	// write/read event to/from bitbuffer
	virtual bool serialize_event(igameevent* event, /*bf_write*/void* buf) = 0;
	virtual igameevent* unserialize_event(/*bf_read*/void* buf) = 0; // create new KeyValues, must be deleted
};

class icvar
{
public:
	// Allocate a unique DLL identifier
	virtual int AllocateDLLIdentifier() = 0;

	// Register, unregister commands
	virtual void			RegisterConCommand(sample_class* pCommandBase) = 0;
	virtual void			UnregisterConCommand(sample_class* pCommandBase) = 0;
	virtual void			UnregisterConCommands(int id) = 0;

	// If there is a +<varname> <value> on the command line, this returns the value.
	// Otherwise, it returns NULL.
	virtual const char* GetCommandLineValue(const char* pVariableName) = 0;

	// Try to find the cvar pointer by name
	virtual sample_class* FindCommandBase(const char* name) = 0;
	virtual const sample_class* FindCommandBase(const char* name) const = 0;
	virtual void idk1() = 0;
	virtual void idk2() = 0;
	virtual void idk3() = 0;
	virtual convar* find_var(const char* var_name) = 0;
	virtual const convar* FindVar(const char* var_name) const = 0;
	virtual sample_class* FindCommand(const char* name) = 0;
	virtual const sample_class* FindCommand(const char* name) const = 0;

	// Get first ConCommandBase to allow iteration
	virtual sample_class* GetCommands(void) = 0;
	virtual const sample_class* GetCommands(void) const = 0;

	// Install a global change callback (to be called when any convar changes) 
	virtual void			InstallGlobalChangeCallback(FnChangeCallback_t callback) = 0;
	virtual void			RemoveGlobalChangeCallback(FnChangeCallback_t callback) = 0;
	virtual void			CallGlobalChangeCallbacks(convar* var, const char* pOldString, float flOldValue) = 0;

	// Install a console printer
	virtual void			InstallConsoleDisplayFunc(sample_class* pDisplayFunc) = 0;
	virtual void			RemoveConsoleDisplayFunc(sample_class* pDisplayFunc) = 0;
	virtual void			ConsoleColorPrintf(const color& clr, const char* pFormat, ...) const = 0;
	virtual void			ConsolePrintf(const char* pFormat, ...) const = 0;
	virtual void			ConsoleDPrintf(const char* pFormat, ...) const = 0;

	// Reverts cvars which contain a specific flag
	virtual void			RevertFlaggedConVars(int nFlag) = 0;

	// Method allowing the engine ICvarQuery interface to take over
	// A little hacky, owing to the fact the engine is loaded
	// well after ICVar, so we can't use the standard connect pattern
	virtual void			InstallCVarQuery(sample_class* pQuery) = 0;
};

class CNetChan;

// thx catalindragan22 (c) // uc
class cclientstate
{
public:
	char pad_0x0000[0x10]; //0x0000
	CNetChan* m_NetChannel; //0x0010 
	char pad_0x0014[0x11C]; //0x0014
	__int32 m_nSignonState; //0x0130 
	char pad_0x0134[0x60]; //0x0134
	__int32 m_nCurrentSequence; //0x0194 
	__int32 unk1; //0x0198 
	__int32 unk2; //0x019C 
	__int32 unk3; //0x01A0 
	char pad_0x01A4[0x10C]; //0x01A4
	__int32 m_nMaxClients; //0x02B0 
	void* N00000583; //0x02B4 
	void* N00000582; //0x02B8 
	char pad_0x02BC[0x4854]; //0x02BC
	float m_flLastServerTickTime; //0x4B10 
	__int32 N0000196E; //0x4B14 
	__int32 oldtickcount; //0x4B18 
	float m_tickRemainder; //0x4B1C 
	float m_frameTime; //0x4B20 
	__int32 lastoutgoingcommand; //0x4B24 
	__int32 chokedcommands; //0x4B28 
	__int32 last_command_ack; //0x4B2C 
	__int32 command_ack; //0x4B30 
	char pad_0x4B34[0x50]; //0x4B34
	Vector viewangles; //0x4B84 
	char pad_0x4B90[0xCB0]; //0x4B90
}; //Size=0x5840

struct surfacephysicsparams_t
{
	// vphysics physical properties
	float			friction;
	float			elasticity;				// collision elasticity - used to compute coefficient of restitution
	float			density;				// physical density (in kg / m^3)
	float			thickness;				// material thickness if not solid (sheet materials) in inches
	float			dampening;
};

struct surfaceaudioparams_t
{
	// sounds / audio data
	float			reflectivity;		// like elasticity, but how much sound should be reflected by this surface
	float			hardnessFactor;	// like elasticity, but only affects impact sound choices
	float			roughnessFactor;	// like friction, but only affects scrape sound choices

	// audio thresholds
	float			roughThreshold;	// surface roughness > this causes "rough" scrapes, < this causes "smooth" scrapes
	float			hardThreshold;	// surface hardness > this causes "hard" impacts, < this causes "soft" impacts
	float			hardVelocityThreshold;	// collision velocity > this causes "hard" impacts, < this causes "soft" impacts
	// NOTE: Hard impacts must meet both hardnessFactor AND velocity thresholds
};

struct surfacesoundnames_t
{
	unsigned short	stepleft;
	unsigned short	stepright;

	unsigned short	impactSoft;
	unsigned short	impactHard;

	unsigned short	scrapeSmooth;
	unsigned short	scrapeRough;

	unsigned short	bulletImpact;
	unsigned short	rolling;

	unsigned short	breakSound;
	unsigned short	strainSound;
};

struct surfacesoundhandles_t
{
	short	stepleft;
	short	stepright;

	short	impactSoft;
	short	impactHard;

	short	scrapeSmooth;
	short	scrapeRough;

	short	bulletImpact;
	short	rolling;

	short	breakSound;
	short	strainSound;
};

struct surfacegameprops_t
{
	// game movement data
	float			maxSpeedFactor;			// Modulates player max speed when walking on this surface
	float			jumpFactor;				// Indicates how much higher the player should jump when on the surface
	// Game-specific data
	unsigned short	material;

	// Indicates whether or not the player is on a ladder.
	unsigned char	climbable;
	unsigned char	pad;
};

struct surfacedata_t
{
	surfacephysicsparams_t	physics;	// physics parameters
	surfaceaudioparams_t	audio;		// audio parameters
	surfacesoundnames_t		sounds;		// names of linked sounds
	surfacegameprops_t		game;		// Game data / properties

	surfacesoundhandles_t		soundhandles;
};

class iphysicssurfaceprops
{
public:
	surfacedata_t* get_surfacedata(int surfaceDataIndex)
	{
		typedef surfacedata_t* (__thiscall* OriginalFn)(PVOID, int);
		return vfunc<OriginalFn>(this, 5)(this, surfaceDataIndex);
	}
};
