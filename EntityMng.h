#pragma once

#include <iostream>

#include <vector>
#include <queue>
#include "EngineType.h"
#include "EngineVar.h"
#include "Logger.h"
#include "ERR.h"

class EntityMng
{
	std::queue<EntityID> id_free;
public:
	Entity entities[MAX_ENTITYID];
	EntityMng(void) {
		std::cout << "INIT" << std::endl;
		for (int i = 0; i < MAX_ENTITYID; i++) {
			this->entities[i] = NOT_ALLOC_COMP_MASK;
			id_free.push(i);
		}
	}
	bool isAlloc(EntityID eid) {
		/*logger->log(new std::string("check alloc start"));
		if(eid>= MAX_ENTITYID) logger->log(new std::string("ffffffffffffffff"));
		else logger->log(new std::string("gggggggggggggggggggggg"));
		if (entities[eid] == NOT_ALLOC_COMP_MASK) return false;
		return true;*/
		return entities[eid] != NOT_ALLOC_COMP_MASK;
	}
	EntityID AllocEntity() {
		EntityID eid;
		if (id_free.empty()) {
			logger->log(new std::string(ERR_MAX_ENTITY));
			// do exception handling
			exit(1);
		}
		eid = id_free.front();
		id_free.pop();
		entities[eid] = 0;
		return eid;
	}
	void FreeEntity(EntityID eid) {
		if (!this->isAlloc(eid)) {
			logger->log(new std::string(ERR_FREE_ENTITY));
			// do exception handling
			exit(1);
		}
		entities[eid] = NOT_ALLOC_COMP_MASK;
		id_free.push(eid);
	}

	void FreeAll() {
		std::queue<EntityID> emp;
		id_free.swap(emp);
		std::cout << "FREE ALL SIZE IS " << id_free.size() << std::endl;
		for (int i = 0; i < MAX_ENTITYID; i++) {
			entities[i] = NOT_ALLOC_COMP_MASK;
			id_free.push(i);
		}
	}
};

