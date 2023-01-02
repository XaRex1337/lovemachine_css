#pragma once
#include "game def's.h"
#include "game classes.h"
#include "game shit.h"
#include "trace shit.h"
#include "math.h"

namespace rage
{
	namespace autowall
	{
		void get_material_params(int a1, float* a2, float* a3)
		{
			//sig = E8 ? ? ? ? 8B 85 ? ? ? ? 83 C4 0C, relative call
			float* result; // eax

			switch (a1)
			{
			case 67:
				*a2 = 0.40000001;
				result = a3;
				*a3 = 0.25;
				break;
			case 68:
			case 77:
				*a2 = 0.5;
				result = a3;
				*a3 = 0.30000001;
				break;
			case 71:
				*a2 = 1.0;
				result = a3;
				*a3 = 0.99000001;
				break;
			case 80:
				*a2 = 0.40000001;
				result = a3;
				*a3 = 0.44999999;
				break;
			case 84:
				*a2 = 0.64999998;
				result = a3;
				*a3 = 0.30000001;
				break;
			case 86:
				*a2 = 0.5;
				result = a3;
				*a3 = 0.44999999;
				break;
			case 87:
				*a2 = 1.0;
				result = a3;
				*a3 = 0.60000002;
				break;
			default:
				*a2 = 1.0;
				result = a3;
				*a3 = 0.5f;
				break;
			}
		}

		/*static void traceline_ignore_two_entities(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask,
			const void* ignore, const void* ignore2, int collisionGroup, trace_t* ptr)
		{
			ray_t ray;
			ray.Init(vecAbsStart, vecAbsEnd);
			CTraceFilterSkipTwoEntities traceFilter(ignore, ignore2, collisionGroup);
			g_pEngineTrace->TraceRay(ray, mask, &traceFilter, ptr);
		}*/


		// GDPR_Anonymous uc <3
		void scale_damage(int nHitGroup, float& flDamage, cweapon* pWeapon)
		{
			switch (nHitGroup)
			{
			case hg_head:
				flDamage *= 4;
				break;

			case hg_chest:
				flDamage *= 1.0;
				break;

			case hg_stomach:
				flDamage *= 1.25;
				break;

			case hg_leftarm:
			case hg_rightarm:
				flDamage *= 1.0;
				break;

			case hg_leftleg:
			case hg_rightleg:
				flDamage *= 0.75;
				break;

			default:
				break;
			}

			if (!this->is_armored(nHitGroup))
			{
				return;
			}

			float flArmorBonus = 0.5f;
			float flArmorRatio = 0.5f;
			size_t wpndata = (size_t)&pWeapon->get_data(); // ??
			flArmorRatio *= *(float*)(wpndata + 0x070C);
			float flNew = flDamage * flArmorRatio;
			float flArmor = (flDamage - flNew) * flArmorBonus;

			// Does this use more armor than we have?
			if (flArmor > this->get_armor())
			{
				//armorHit = (int)(flArmor);

				flArmor = this->get_armor();
				flArmor *= (1 / flArmorBonus);
				flNew = flDamage - flArmor;
			}

			flDamage = flNew;
		}

		bool trace_to_exit(Vector& start, Vector& dir, Vector& end, float flStepSize, float flMaxDistance)
		{
			float flDistance = 0;
			Vector last = start;

			while (flDistance <= flMaxDistance)
			{
				flDistance += flStepSize;

				end = start + Vector(flDistance, flDistance, flDistance) * dir;

				if ((_engine_trace->get_point_contents(end, nullptr) & MASK_SOLID) == 0)
				{
					return true;
				}
			}

			return false;
		}

		int get_damage(Vector source, const qangle& angles, centity* pThis, centity* pTarget, int mindmg, int maxpen, bool* bVisible = nullptr, float init_vis_distance = 0.f, int init_hitgroup = hg_generic)
		{
			cweapon* pWeapon = (cweapon*)pThis->get_weapon();
			if (!pWeapon)
			{
				return 0.f;
			}

			Vector vecDirShooting;
			anglevectors(angles, &vecDirShooting);

			float flPenetrationPower = 0.0;
			float flPenetrationDistance = 0.0;
			float flDamageModifier = 1.f;
			float flPenetrationModifier = 1.0;

			size_t wpndata = (size_t)&pWeapon->get_data();
			float flDistance = *(float*)(wpndata + 0x088C);
			int iDamage = *((int*)wpndata + 546);
			float flRangeModifier = *((float*)wpndata + 548);
			int iPenetration;
			if (maxpen == 0/*AUTOWALL_PEN_TO_MINDMG*/) // ???
			{
				iPenetration = *((int*)wpndata + 432);
			}
			else
			{
				iPenetration = min(*((int*)wpndata + 432), maxpen);
			}
			int iBulletType = *(int*)(wpndata + 0x0894);
			int shots = *(int*)(wpndata + 549 * 4);
			if (shots <= 0)
			{
				return 0.f;
			}

			pThis->get_bullettype_parameters(iBulletType, flPenetrationPower, flPenetrationDistance);

			float fCurrentDamage = iDamage;
			float flCurrentDistance = 0.f;

			centity* lastPlayerHit = nullptr;

			if (*(int*)((size_t)pWeapon + 0x92C) == 1)
			{
				switch (pWeapon->get_weaponid())
				{
				case 2:
					fCurrentDamage = 18;
					flRangeModifier = 0.89999998;

					break;
				case 21:
					flRangeModifier = 0.94999999;
					break;
				case 16:
					fCurrentDamage = 30;
					break;
				}
			}

			//THIS ASSUMES THAT POINT IS VISIBLE TO OPTIMIZE TRACES
			if (bVisible && *bVisible && init_vis_distance > 0.f && init_hitgroup != hg_generic && init_hitgroup != hg_gear)
			{
				fCurrentDamage *= pow(flRangeModifier, init_vis_distance * 0.002f);
				pTarget->scale_damage(init_hitgroup, fCurrentDamage, pWeapon);
				return fCurrentDamage;
			}

			bool bFirstTrace = true;
			const float flTraceToExitDist = g_pSharedCon->m_pMaxTraceToExit->Value();
			const float flTraceToExitJump = g_pSharedCon->m_pTraceToExitJump->Value();

			while ((int)fCurrentDamage > mindmg)
			{
				Vector vecEnd = source + vecDirShooting * flDistance;
				trace_t tr;

				UTIL_TraceLineIgnoreTwoEntities(source, vecEnd, 1174421515, pThis, lastPlayerHit, 0 /*COLLISION_GROUP_NONE*/, &tr);
				{
					CTraceFilterSkipTwoEntities filter(pThis, lastPlayerHit, 0 /*COLLISION_GROUP_NONE*/);
					UTIL_ClipTraceToPlayers(source, vecEnd + vecDirShooting * 40.f, 1174421515, &filter, &tr);
				}

				if (tr.fraction == 1.f)
				{
					fCurrentDamage = 0.f;
					break;
				}

				flCurrentDistance += tr.fraction * flDistance;
				fCurrentDamage *= pow(flRangeModifier, flCurrentDistance * 0.002f);

				if (tr.m_pEnt == pTarget && tr.hitgroup != hg_generic && tr.hitgroup != hg_gear)
				{
					pTarget->scale_damage(tr.hitgroup, fCurrentDamage, pWeapon);
					if (bVisible)
					{
						*bVisible = bFirstTrace;
					}

					break;
				}

				lastPlayerHit = tr.m_pEnt;

				surfacedata_t* pSurfaceData = g_pPhysicsSurfaceProps->GetSurfaceData(tr.surface.surfaceProps);
				int iEnterMaterial = pSurfaceData->game.material;

				GetMaterialParameters(iEnterMaterial, &flPenetrationModifier, &flDamageModifier);
				bool v34 = tr.contents & CONTENTS_GRATE;
				if (v34)
				{
					flPenetrationModifier = 1.0f;
					flDamageModifier = 0.99f;
				}

				if (flCurrentDistance > flPenetrationDistance && iPenetration > 0)
					iPenetration = 0;

				if (iPenetration == 0 && !v34)
				{
					fCurrentDamage = 0.f;
					break;
				}

				if (iPenetration < 0)
				{
					fCurrentDamage = 0.f;
					break;
				}

				Vector penetrationEnd;

				if (!TraceToExit(tr.endpos, vecDirShooting, penetrationEnd, flTraceToExitJump, flTraceToExitDist))
				{
					fCurrentDamage = 0.f;
					break;
				}

				trace_t exitTr;
				UTIL_TraceLine(penetrationEnd, tr.endpos, 1174421515, 0, &exitTr);

				if (exitTr.m_pEnt != tr.m_pEnt && exitTr.m_pEnt != NULL)
				{
					UTIL_TraceLine(penetrationEnd, tr.endpos, 1174421515, exitTr.m_pEnt, COLLISION_GROUP_NONE, &exitTr);
				}

				pSurfaceData = g_pPhysicsSurfaceProps->GetSurfaceData(exitTr.surface.surfaceProps);
				int iExitMaterial = pSurfaceData->game.material;

				if (iEnterMaterial == iExitMaterial && (iExitMaterial == 87 || iExitMaterial == 77))
				{
					flPenetrationModifier *= 2.f;
				}

				float flTraceDistance = VectorLength(exitTr.endpos - tr.endpos);
				if (flTraceDistance > (flPenetrationPower * flPenetrationModifier))
				{
					fCurrentDamage = 0.f;
					break;
				}

				flPenetrationPower -= flTraceDistance / flPenetrationModifier;
				flCurrentDistance += flTraceDistance;

				source = exitTr.endpos;
				flDistance = (flDistance - flCurrentDistance) * 0.5;

				fCurrentDamage *= flDamageModifier;
				iPenetration--;
				bFirstTrace = false;
			}

			fCurrentDamage *= shots;
			return fCurrentDamage;
		}
	}
}