#pragma once

#include <math.h>

#include "EntityMng.h"

#include "BaseAction.h"
#include "CompTransform.h"
#include "CompMobile.h"

#include "collisionCalc.h"

#include "Logger.h"

struct Data_ActionPhysics {
	float delta;
	CompTransform* compTrans;
	CompMobile* compMobile;
	CellMng* cellMng;
};

class ActionPhyics : public BaseAction
{
	Data_ActionPhysics* _data;
public:
	void start(void* data) {
		_data = (Data_ActionPhysics*)data;
		for (int i = 0; i < MAX_ENTITYID; i++) {
			if (!test(i)) continue;
			action(i);
		}
	};
	void action(EntityID eid) {
		auto data = _data;

		auto delta = data->delta;
		auto mass = data->compMobile->mass;
		auto pos = data->compTrans->pos;
		auto collisionChecked = data->compMobile->collisionChecked;
		auto force = data->compMobile->force;
		auto vel = data->compMobile->vel;
		auto cellMng = data->cellMng;

		auto old_cellNum = cellMng->getCellNum(pos[eid][0], pos[eid][1], pos[eid][2]);
		if (!collisionChecked[eid]) {
			std::vector<EntityID> eids;
			cellMng->getEIDinCell(eids, old_cellNum);
			if (eids.size() > 1) {
				for (auto iter = eids.begin(); iter != eids.end(); iter++) {
					EntityID id = *iter;
					float dist;
					if (((id) == eid) || (collisionChecked[id])) continue;
					dist = dist_s2s(pos[eid], pos[id]);
					if (dist > SIZE_PARTICLE) continue;
					collisionChecked[eid] = collisionChecked[id] = true;
					float vZMF[3];
					convertZMF(vel[eid], vel[id], vZMF);
					calcCollision(vel[eid], vel[id], pos[eid], pos[id]);
					recoverZMF(vel[eid], vel[id], vZMF);
					//std::cout << "COLLISION!!!" << std::endl;
					break;
				}
			}
		}
		float dV[3];
		float dS[3];
		float acc_movement[3];
		float tmp[3];

		vec3Mul(vel[eid], delta, dS);
		vec3Mul(force[eid], delta * delta / 2.0f, acc_movement);
		vec3Add(pos[eid], dS, pos[eid]);
		vec3Add(pos[eid], acc_movement, pos[eid]);

		vec3Mul(force[eid], delta, dV);
		vec3Add(vel[eid], dV, vel[eid]);

		if (pos[eid][0] < SIZE_PARTICLE) {
			pos[eid][0] = SIZE_PARTICLE;
			vel[eid][0] *= -wall_slowing;
			if (abs(vel[eid][0]) < stop_vel) vel[eid][0] = 0.0f;
		}
		else if (pos[eid][0] > (MAX_DIST - SIZE_PARTICLE)) {
			pos[eid][0] = (MAX_DIST - SIZE_PARTICLE);
			vel[eid][0] *= -wall_slowing;
			if (abs(vel[eid][0]) < stop_vel) vel[eid][0] = 0.0f;
		}
		if (pos[eid][1] < SIZE_PARTICLE) {
			pos[eid][1] = SIZE_PARTICLE;
			vel[eid][1] *= -wall_slowing;
			if (abs(vel[eid][1]) < stop_vel) vel[eid][1] = 0.0f;
		}
		else if (pos[eid][1] > (MAX_DIST - SIZE_PARTICLE)) {
			pos[eid][1] = (MAX_DIST - SIZE_PARTICLE);
			vel[eid][1] *= -wall_slowing;
			if (abs(vel[eid][1]) < stop_vel) vel[eid][1] = 0.0f;
		}
		if (pos[eid][2] < SIZE_PARTICLE) {
			pos[eid][2] = SIZE_PARTICLE;
			vel[eid][2] *= -wall_slowing;
			if (abs(vel[eid][2]) < stop_vel) vel[eid][2] = 0.0f;
		}
		else if (pos[eid][2] > (MAX_DIST - SIZE_PARTICLE)) {
			pos[eid][2] = (MAX_DIST - SIZE_PARTICLE);
			vel[eid][2] *= -1.0f;
			if (abs(vel[eid][2]) < stop_vel) vel[eid][2] = 0.0f;
		}

		auto new_cellNum = cellMng->getCellNum(pos[eid][0], pos[eid][1], pos[eid][2]);
		if (old_cellNum != new_cellNum) {
			cellMng->removeEntity(eid, old_cellNum);
			cellMng->addEntity(eid, new_cellNum);
		}
	}
};

