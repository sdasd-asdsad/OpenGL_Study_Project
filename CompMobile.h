#pragma once

#include "BaseComponent.h"
#include "EngineVar.h"

class CompMobile : public BaseComponent
{
public:
	float mass[MAX_ENTITYID];
	float vel[MAX_ENTITYID][3];
	float force[MAX_ENTITYID][3];
	int cellNumber[MAX_ENTITYID];
	bool collisionChecked[MAX_ENTITYID];
};

