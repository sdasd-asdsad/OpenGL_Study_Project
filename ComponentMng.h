#pragma once

#include <queue>

#include "EntityMng.h"
#include "BaseComponent.h"

#include "EngineType.h"
#include "EngineVar.h"
#include "Logger.h"
#include "ERR.h"


class ComponentMng
{
	std::queue<CompID> id_free;
public:
	BaseComponent* components[MAX_COMPONENTID];
	ComponentMng(void) {
		for (int i = 0; i < MAX_COMPONENTID; i++) {
			components[i] = nullptr;
			id_free.push(i);
		}
	}
	inline bool hasComponent(Entity entity, CompID cid) {
		// assume the entity is allocated
		return ((entity & (1 << cid)) != 0);
	}
	inline bool hasAllRequired(Entity entity, Entity mask) {
		return ((entity & mask) == mask);
	}
	CompID Register(BaseComponent* comp) {
		if (id_free.empty()) {
			logger->log(new std::string(ERR_MAX_COMPONENT));
			// do exception handling
			exit(1);
		}
		CompID cid = id_free.front();
		comp->id = cid;
		id_free.pop();
		return cid;
	}
	void AssignComp(EntityMng* eMng_ptr, EntityID eid, CompID cid) {
		Entity entity = eMng_ptr->entities[eid];
		if (!eMng_ptr->isAlloc(eid)) {
			// do exception handling
			exit(1);
		}
		else if (hasComponent(entity,cid)) {
			// do exception handling
			exit(1);
		}
		eMng_ptr->entities[eid] |= (1 << cid);
	}
};

