#pragma once

#include "EntityMng.h"

#include "BaseAction.h"
#include "CompTransform.h"
#include "CompMobile.h"

#include "Logger.h"

struct Data_ActionPhysics {
	float delta;
	CompTransform* compTrans;
	CompMobile* compMobile;
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
		auto force = data->compMobile->force;
		auto vel = data->compMobile->vel;
		
		auto dVx = delta * (force[eid][0]) / mass;
		auto dVy = delta * (force[eid][1]) / mass;
		auto dVz = delta * (force[eid][2]) / mass;

		pos[eid][0] += delta * (vel[eid][0]) + delta * dVx / 2;
		pos[eid][1] += delta * (vel[eid][1]) + delta * dVy / 2;
		pos[eid][2] += delta * (vel[eid][2]) + delta * dVz / 2;

		vel[eid][0] += dVx;
		vel[eid][1] += dVy;
		vel[eid][2] += dVz;
	}
};

