#pragma once
#include "includes.h"
#include "vector.h"
#include "game def's.h"

struct model_t;
struct studiohdr_t;
struct studiohwdata_t;
struct StudioDecalHandle_t { int unused; };
typedef unsigned short ModelInstanceHandle_t;

enum
{
	MODEL_INSTANCE_INVALID = (ModelInstanceHandle_t)~0
};

class iclientrenderable
{
public:
	const model_t* get_model()
	{
		typedef const model_t*(__thiscall* get_model_fn)(void*);
		return vfunc< get_model_fn >(this, 9)(this);
	}

	// Call this to get the current bone transforms for the model.
	// currentTime parameter will affect interpolation
	// nMaxBones specifies how many matrices pBoneToWorldOut can hold. (Should be greater than or
	// equal to studiohdr_t::numbones. Use MAXSTUDIOBONES to be safe.)
	bool setup_bones(matrix3x4_t* bone_to_world_out, int max_bones, int bone_mask, float current_time)
	{
		typedef bool(__thiscall* setup_bones_fn)(void*, matrix3x4_t*, int, int, float);
		return vfunc< setup_bones_fn >(this, 16)(this, bone_to_world_out, max_bones, bone_mask, current_time);
	}
};

struct ModelRenderInfo_t
{
	cvector origin;
	qangle angles;
	iclientrenderable* pRenderable;
	const model_t* pModel;
	const matrix3x4_t* pModelToWorld;
	const matrix3x4_t* pLightingOffset;
	const Vector* pLightingOrigin;
	int flags;
	int entity_index;
	int skin;
	int body;
	int hitboxset;
	ModelInstanceHandle_t instance;

	ModelRenderInfo_t()
	{
		pModelToWorld = NULL;
		pLightingOffset = NULL;
		pLightingOrigin = NULL;
	}
};

struct DrawModelState_t
{
	studiohdr_t* m_pStudioHdr;
	studiohwdata_t* m_pStudioHWData;
	iclientrenderable* m_pRenderable;
	const matrix3x4_t* m_pModelToWorld;
	StudioDecalHandle_t		m_decals;
	int						m_drawFlags;
	int						m_lod;
};