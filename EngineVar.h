#pragma once

#include "EngineType.h"
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>

#define MAX_ENTITYID (1024)
#define MAX_COMPONENTID (30) // should be 2^n-1

#define NOT_ALLOC_COMP_MASK ((Entity)(1<<MAX_COMPONENTID))

#define SIZE_PARTICLE (0.1f)
#define MAX_DIST      (5.0f)
#define NUM_PARTICLE  (512)
#define wall_slowing (0.9f)
#define stop_vel (0.001f)
#define CEEL_PARTITION (10)

#define PI (3.141592)

#define MAX_SPEED_PARTICLE (5.0f)

unsigned int INIT_W = 1280;
unsigned int INIT_H = 720;

#define deg2rad(deg) (PI/180.0f * (float)deg)

GLint view_long = 0.0;
GLint view_lat = 90;
GLfloat view_rad = 5.0f;
GLfloat view_sensitivity = 0.5f;