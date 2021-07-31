#pragma once

#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/GL.h>
#include <GL/GLU.h>

#include "BaseComponent.h"
#include "EngineVar.h"

class CompMaterial : public BaseComponent {
public:
	GLfloat diff[MAX_ENTITYID][4];
	GLfloat ambient[MAX_ENTITYID][4];
	GLfloat specular[MAX_ENTITYID][4];
	GLfloat shininess[MAX_ENTITYID][4];

};