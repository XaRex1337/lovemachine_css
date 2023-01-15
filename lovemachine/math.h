#pragma once
#include "includes.h"
#include "vector.h"

#define M_PI 3.14159265358979323846f
#define rad2deg( x )  ( (float)(x) * (float)(180.f / (float)M_PI) )
#define deg2rad( x )  ( (float)(x) * (float)((float)M_PI / 180.f) )

void normalize_angle(qangle& angle)
{
	//while (angle.x > 89.f) angle.x -= 180.f;
	//while (angle.x < -89.f) angle.x += 180.f;
	angle.x = max(-89.f, min(angle.x, 89.f));
	while (angle.y > 180.f) angle.y -= 360.f;
	while (angle.y < -180.f) angle.y += 360.f;
}

void vectorangles(const cvector& forward, qangle& angles)
{
	if (forward[1] == 0.0f && forward[0] == 0.0f)
	{
		angles[0] = (forward[2] > 0.0f) ? 270.0f : 90.0f;
		angles[1] = 0.0f;
	}
	else
	{
		angles[0] = atan2(-forward[2], forward.Length2D()) * -180 / M_PI;
		angles[1] = atan2(forward[1], forward[0]) * 180 / M_PI;

		if (angles[1] > 90)
			angles[1] -= 180;
		else if (angles[1] < 90)
			angles[1] += 180;
		else if (angles[1] == 90)
			angles[1] = 0;
	}

	angles[2] = 0.0f;
}

void sincos(float radians, float* sine, float* cosine)
{
	*sine = sinf(radians);
	*cosine = cosf(radians);
}

void anglevectors(const qangle& angles, cvector* forward)
{
	float sp, sy, cp, cy;

	sincos(deg2rad(angles.y), &sy, &cy);
	sincos(deg2rad(angles.x), &sp, &cp);

	forward->x = cp * cy;
	forward->y = cp * sy;
	forward->z = -sp;
}

qangle calc_angle(cvector src, cvector dst)
{
	qangle angles;
	cvector delta = src - dst;
	vectorangles(delta, angles);
	delta.Normalize();
	//normalize_angle(delta); // delta.Normalize(); зачем нужна эта функция здесь?
	return angles;
}

float get_fov(const qangle& viewAngle, const qangle& aimAngle)
{
	Vector ang, aim;

	anglevectors(viewAngle, &aim);
	anglevectors(aimAngle, &ang);

	return rad2deg(acos(aim.Dot(ang) / aim.LengthSqr()));
}

void vector_transform(cvector in, matrix3x4_t transform, cvector& out)
{
	for (int i = 0; i < 3; i++)
		out[i] = in.Dot(transform[i]) + transform[i][3];
}

template<typename t>
t clamp(t pmin, t val, t pmax)
{
	return max(pmin, min(val, pmax));
}