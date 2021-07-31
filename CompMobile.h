#pragma once

#include "BaseComponent.h"
#include "EngineVar.h"

class CompMobile : public BaseComponent
{
public:
	float mass;
	float vel[MAX_ENTITYID][3];
	float force[MAX_ENTITYID][3];
};

