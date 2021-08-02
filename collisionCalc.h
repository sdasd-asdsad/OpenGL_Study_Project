#pragma once

#include <math.h>
#include <vector>

#include "EngineType.h"

#ifndef max
#define max(a,b)  (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)  (((a) < (b)) ? (a) : (b))
#endif

inline float dist_s2s(float x1, float y1, float z1, float x2, float y2, float z2) {
	auto dv = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2);
	return sqrtf(dv);
}

inline float dist_s2s(float* a, float* b) {
	return dist_s2s(a[0], a[1], a[2], b[0], b[1], b[2]);
}

inline int get3dto1d(int N, int XN, int YN, int ZN) {
	return XN + N * YN + N * N * ZN;
}

float quick_isqrt(float number) 
// https://github.com/id-Software/Quake-III-Arena/blob/master/code/game/q_math.c#L552
// Thanks quake!
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y = number;
	i = *(long*)&y;
	i = 0x5f3759df - (i >> 1);
	y = *(float*)&i;
	y = y * (threehalfs - (x2 * y * y));

	return y;
}

inline float norm2sqr(float* a) {
	return a[0] * a[0] + a[1] * a[1] + a[2] * a[2];
}

void normalize(float* a) {
	float n;
	n = norm2sqr(a);
	n = quick_isqrt(n);
	a[0] *= n;
	a[1] *= n;
	a[2] *= n;
	return;
}

inline void vec3Sub(float* a, float* b, float* c) {
	c[0] = a[0] - b[0];
	c[1] = a[1] - b[1];
	c[2] = a[2] - b[2];
	return;
}

inline void vec3Add(float* a, float* b, float* c) {
	c[0] = a[0] + b[0];
	c[1] = a[1] + b[1];
	c[2] = a[2] + b[2];
	return;
}

inline void vec3Mul(float* a, float b, float* c) {
	c[0] = a[0] * b;
	c[1] = a[1] * b;
	c[2] = a[2] * b;
	return;
}

inline float vec3dot(float* a, float* b) {
	return (a[0] * b[0] + a[1] * b[1] + a[2] * b[2]);
}

void convertZMF(float* a, float* b, float* c) {
	vec3Add(a, b, c);
	vec3Mul(c, 0.5f, c);
	// vector c is now Vzmf
	vec3Sub(a, c, a);
	vec3Sub(b, c, b);
}

void recoverZMF(float* a, float* b, float* c) {
	vec3Add(a, c, a);
	vec3Add(b, c, b);
}

void calcCollision(float* va, float* vb, float* a, float* b) {
	float d[3];
	float vd[3];
	vec3Sub(b, a, d);
	float va_d_dot;
	normalize(d);
	va_d_dot = vec3dot(va, d);
	if (va_d_dot < 0.0f) return; // what's going on??
	// va + vb = (va-vd) + (vb+vd)
	// vd = 2 va*d d
	vec3Mul(d, 2.0f * va_d_dot, vd);
	vec3Sub(va, vd, va);
	vec3Add(vb, vd, vb);
}

class Cell {
public:
	std::vector<Cell*> adjCells;
	std::vector<EntityID> entities;
};

class CellMng {
	int N;
	int numCells;
public:
	float interval;
	std::vector<Cell*> cells;
	CellMng(int _N = CEEL_PARTITION){
		this->N = _N;
		this->interval = MAX_DIST / (float)this->N;
		this->numCells = this->N * this->N * this->N;
		for (int i = 0; i < this->numCells; i++) {
			cells.push_back(new Cell());
		}
	}
	void initCells() {
		for (int i = 0; i < this->numCells; i++) {
			cells[i]->entities.clear();
		}
	}
	int getCellNum(float x, float y, float z) {
		int XN = floorf(x / this->interval);
		int YN = floorf(y / this->interval);
		int ZN = floorf(z / this->interval);

		return XN + N * YN + N * N * ZN;
	}
	void addEntity(EntityID eid, int cellNumber) {
		cells[cellNumber]->entities.push_back(eid);
	}
	void removeEntity(EntityID eid, int cellNumber) {
		std::vector<EntityID>::iterator it;
		for (it = cells[cellNumber]->entities.begin();
			it != cells[cellNumber]->entities.end();) {
			if ((*it) == eid) {
				cells[cellNumber]->entities.erase(it);
				break;
			}
		}
	}
	void getQuantized3d(std::vector<int> &vec3_pos,int cellNumber) {
		int tmp = cellNumber;
		auto XN = tmp % this->N;
		//tmp = (tmp - XN)/this->N;
		tmp = tmp / this->N;
		auto YN = tmp % this->N;
		//tmp = (tmp - YN) / this->N;
		tmp = tmp / this->N;
		auto ZN = tmp;
		vec3_pos.push_back(XN);
		vec3_pos.push_back(YN);
		vec3_pos.push_back(ZN);
	}
	void getEIDinCell(std::vector<EntityID>& eids, int cellNumber) {
		eids = this->cells[cellNumber]->entities;
	}
	/*
	void genAdjEntities() {
		std::vector<int> vec3_pos;
		for (int i = 0; i < this->numCells; i++) {
			vec3_pos.clear();
			getQuantized3d(vec3_pos,i);
			auto xmin = min(0, vec3_pos[0] - 1);
			auto xmax = max(this->N, vec3_pos[0] + 2);
		}
	}*/
};