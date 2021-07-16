#pragma once
#include <vector>
#include "ECS.h"

struct physics_param {
	ComponentID physicID;
	ComponentID transformID;
	float deltaTime;
};

struct control_param {
	ComponentID physicID;
	ComponentID transformID;
	uint8_t k;
};

class Transform
{
public:
	vector<float> pos;

	Transform() { pos.push_back(0.0f); pos.push_back(0.0f); pos.push_back(0.0f); }
	// add rotation information here.
};

class Physics
{
public:
	float mass;
	vector<float> force;
	vector<float> vel;
	// add rotation information here.

	Physics() {
		force.push_back(0.0f); force.push_back(0.0f); force.push_back(0.0f);
		vel.push_back(0.0f); vel.push_back(0.0f); vel.push_back(0.0f);
		mass = 1.0f;
	}
};

class Controller
{

};

class Scene
{
public:
	ComponentID transformID;
	ComponentID physicsID;
	ComponentID controllerID;

	EntityMng entityMng;
	ComponentMng compMng;

	Scene();

	void updatePhysics(float deltaTime) {
		param_physics.deltaTime = deltaTime;
		entityMng.update(physics_mask, &compMng, physics_system, (void*)(&param_physics));
	}

	EntityID AddEntity() {
		EntityID eid = entityMng.AllocEntity();
		return eid;
	}

	void AssignComponent(EntityID eid, ComponentID cid) {
		entityMng.AssignComponent(eid, cid);
	}
private:

	physics_param param_physics;
	control_param param_control;
	
	Signature physics_mask;
	void (*physics_system)(ComponentMng*, EntityID, void*);

	Signature control_mask;
	void (*control_system)(ComponentMng*, EntityID, void*);
};

void physics(ComponentMng* ptr_compMng, EntityID eid, void* val) {
	auto param = ((physics_param*)val);

	// change vel
	Component<Physics>* ptr_physics = (Component<Physics>*)(ptr_compMng->GetComponent(param->physicID));
	Component<Transform>* ptr_transform = (Component<Transform>*)(ptr_compMng->GetComponent(param->transformID));
	auto deltaTime = param->deltaTime;
	auto mass = ptr_physics->Entities[eid].mass;

	auto dVx = deltaTime * ptr_physics->Entities[eid].force[0] / mass;
	auto dVy = deltaTime * ptr_physics->Entities[eid].force[1] / mass;
	auto dVz = deltaTime * ptr_physics->Entities[eid].force[2] / mass;

	ptr_transform->Entities[eid].pos[0] += deltaTime * ptr_physics->Entities[eid].vel[0] + deltaTime * dVx / 2;
	ptr_transform->Entities[eid].pos[1] += deltaTime * ptr_physics->Entities[eid].vel[1] + deltaTime * dVy / 2;
	ptr_transform->Entities[eid].pos[2] += deltaTime * ptr_physics->Entities[eid].vel[2] + deltaTime * dVz / 2;

	ptr_physics->Entities[eid].vel[0] += dVx;
	ptr_physics->Entities[eid].vel[1] += dVy;
	ptr_physics->Entities[eid].vel[2] += dVz;
}

Scene::Scene()
{
	auto cid_transform = compMng.AllocComponent();
	auto ptr_transform = new Component<Transform>(cid_transform);
	compMng.AddComponent(cid_transform, ptr_transform);
	transformID = cid_transform;

	auto cid_physics = compMng.AllocComponent();
	auto ptr_physics = new Component<Physics>(cid_physics);
	compMng.AddComponent(cid_physics, ptr_physics);
	physicsID = cid_physics;

	auto cid_control = compMng.AllocComponent();
	auto ptr_control = new Component<Controller>(cid_control);
	compMng.AddComponent(cid_control, ptr_control);
	controllerID = cid_control;

	physics_mask = (1 << transformID) | (1 << physicsID);
	control_mask = (1 << transformID) | (1 << physicsID) | (1 << controllerID);

	param_physics.physicID = physicsID;
	param_physics.transformID = transformID;

	physics_system = physics;
}
