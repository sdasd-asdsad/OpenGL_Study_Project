#pragma once

#include "BaseAction.h"
#include <random>

struct Data_ActionRegen {
	CompTransform* compTrans;
	CompMobile* compMobile;
	CompRegen* compRegen;
};

class ActionRegen : public BaseAction
{
	Data_ActionRegen* data;
public:
	void start(void* _data) {
		this->data = (Data_ActionRegen*)_data;
		for (int i = 0; i < MAX_ENTITYID; i++) {
			if (!test(i)) continue;
			action(i);
		}
	};
	void action(EntityID eid) {
		auto limit = data->compRegen->MIN_Y;

		if (data->compTrans->pos[eid][1] < limit) {
			data->compMobile->vel[eid][1] = -1.0f-((float)rand()/(float)RAND_MAX);
			data->compTrans->pos[eid][1] = 4.0f;
		}
	}
};

