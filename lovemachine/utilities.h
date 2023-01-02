#pragma once
#include "game classes.h"
#include "trace shit.h"

bool is_visible(centity* entity, cvector to)
{
	if (to.IsZero()) return false;

	cvector from = global::local_observed->get_eye_pos();
	if (from.IsZero()) return false;

	itracefilter filter;
	filter.skip = global::local_observed;
	filter.target = entity;

	ray_t ray; ray.Init(from, to);

	trace_t trace;

	_engine_trace->trace_ray(ray, MASK_SOLID/*MASK_ALL*/, &filter, &trace);

	return (trace.m_pEnt == entity && !trace.allsolid);
}

bool is_hitbox_visible(centity* entity, int id = 0, matrix3x4_t* matrix = nullptr)
{
	return is_visible(entity, entity->get_hitbox(id, matrix, global::curtime));
}