#pragma once
#include <stdint.h>
#include <vector>
#include <queue>

#define MAX_EntityID (1024)
#define MAX_ComponentID (30)

using EntityID = uint32_t;
using Signature = uint32_t;

using ComponentID = uint8_t;

using namespace std;

class _component {
protected:
	ComponentID CID;
};

template <typename T>
class Component : public _component {
public:
	Component(ComponentID);
	ComponentID getCID();
	T Entities[MAX_EntityID];
};

class ComponentMng {
public:
	ComponentMng();
	ComponentID AllocComponent();
	void AddComponent(ComponentID, _component*);
	_component* GetComponent(ComponentID);
	int component_cnt;
	_component* components[MAX_ComponentID];
};

class EntityMng {
public:
	EntityMng();
	EntityID AllocEntity();
	int AssignComponent(EntityID, ComponentID);
	int FreeEntity(EntityID);
	inline bool isAllocated(EntityID);

	int update(Signature sig, ComponentMng* ptr_compMng, void(*f)(ComponentMng*, EntityID, void*), void* val);

	Signature Entities[MAX_EntityID];
	queue<EntityID> idle_EIDs;
};




EntityMng::EntityMng()
{
	for (int i = 0; i < MAX_EntityID; i++) {
		idle_EIDs.push(i);
		Entities[i] = 1 << MAX_ComponentID;
	}
}

EntityID EntityMng::AllocEntity()
{
	EntityID eid;
	eid = idle_EIDs.front();
	idle_EIDs.pop();
	Entities[eid] = (Signature)0;
	return eid;
}

int EntityMng::AssignComponent(EntityID eid, ComponentID cid)
{
	if (!isAllocated(eid)) { 
		cout << __FILE__ << "-" << __LINE__ << " : " << Entities[eid] << "  = " << isAllocated(eid) << endl;
		return -1; 
	} // not allocated eid.
	Entities[eid] |= (1 << cid);
	cout << __FILE__ << "-" << __LINE__ << " : " << Entities[eid] << endl;
	return 0;
}

int EntityMng::FreeEntity(EntityID eid)
{
	if (~isAllocated(eid)) return -1; // not allocated eid.
	Entities[eid] = 1 << MAX_ComponentID;
	idle_EIDs.push(eid);
	return eid;
}

inline bool EntityMng::isAllocated(EntityID eid)
{
	return Entities[eid] != (1 << MAX_ComponentID);
}

int EntityMng::update(Signature sig, ComponentMng* ptr_compMng, void (*f)(ComponentMng*, EntityID, void*), void* val)
{
	for (int i = 0; i < MAX_EntityID; i++) {
		if ((Entities[i] & sig) != sig) continue;
		f(ptr_compMng, i, val);
	}
	return 0;
}

template<typename T>
Component<T>::Component(ComponentID cid)
{
	CID = cid;
}

template<typename T>
ComponentID Component<T>::getCID()
{
	return CID;
}



ComponentMng::ComponentMng()
{
	component_cnt = 0;
}

ComponentID ComponentMng::AllocComponent() {
	component_cnt++;
	return component_cnt - 1;
}

void ComponentMng::AddComponent(ComponentID cid, _component* ptr_comp)
{
	components[cid] = ptr_comp;
}

_component* ComponentMng::GetComponent(ComponentID cid)
{
	if (cid >= component_cnt) return nullptr;
	return components[cid];
}
