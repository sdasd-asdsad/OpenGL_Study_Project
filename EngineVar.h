#pragma once

#include "EngineType.h"
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>

#define MAX_ENTITYID (1024)
#define MAX_COMPONENTID (30) // should be 2^n-1

#define NOT_ALLOC_COMP_MASK ((Entity)(1<<MAX_COMPONENTID))

unsigned int INIT_W = 640;
unsigned int INIT_H = 480;

//GLint INIT_W = 640;