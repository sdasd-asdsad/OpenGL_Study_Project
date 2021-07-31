#pragma once

#include <vector>
#include "EngineType.h"
#include "EntityMng.h"
#include "ComponentMng.h"

#include "Logger.h"

class BaseAction {
public:
	Entity required;
	EntityMng* eMng;
	ComponentMng* cMng;
	bool test(EntityID eid) {
		Entity entity;
		if (!eMng->isAlloc(eid))
			return false;
		entity = eMng->entities[eid];
		return cMng->hasAllRequired(entity,this->required);
	}
	virtual void start(void* data) = 0;
	virtual void action(EntityID eid) = 0;
};