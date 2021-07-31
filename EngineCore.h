#pragma once

#include <vector>
#include <random>
#include <iostream>

#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/GL.h>
#include <GL/GLU.h>

#include "EntityMng.h"
#include "ComponentMng.h"

#include "BaseComponent.h"
#include "CompTransform.h"
#include "CompMobile.h"
#include "CompMaterial.h"
#include "CompRegen.h"

#include "BaseAction.h"
#include "ActionPhyics.h"
#include "ActionRegen.h"

#include "EngineType.h"
#include "EngineVar.h"
#include "Logger.h"
#include "ERR.h"


const int MAIN_SCENE = 0;
int sceneID = -1;
int new_sceneID = MAIN_SCENE;

// Managers.....
EntityMng eMng;
ComponentMng cMng;
///////////////////////

// COMPONENT DECLARATION
CompTransform compTrans;
CompID cid_trans;

CompMobile compMobile;
CompID cid_mobile;

CompMaterial compMat;
CompID cid_mat;

CompRegen compRegen;
CompID cid_regen;
////////////////////////

// ACTION CONTAINER
std::vector<BaseAction> actions;
ActionPhyics actionPhysics;
ActionRegen actionRegen;


// timer
int old_time;

GLfloat mat_sun_diff[] = { 0.93,0.21,0.06,1.0 };
GLfloat mat_non_diff[] = { 1.0,1.0,1.0,1.0 };
GLfloat mat_sun_ambient[] = { 0.93,0.21,0.06,1.0 };
GLfloat mat_specular[] = { 1.0,1.0,1.0,1.0 };
GLfloat mat_shininess[] = { 50.0 };

void modelMaterial(GLfloat* diff, GLfloat* ambient) {
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}



void RegisterComponent() {
	// transform
	cid_trans = cMng.Register(&compTrans);	
	cid_mobile = cMng.Register(&compMobile);
	cid_mat = cMng.Register(&compMat);
	cid_regen = cMng.Register(&compRegen);
}

void draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(-2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	modelMaterial(mat_sun_diff, mat_sun_ambient);
	glPushMatrix();
	glRotatef(90.0, 1.0, 0, 0);
	glutSolidTorus(0.1, 0.5, 20, 30);
	glPopMatrix();

	for (int i = 0; i < MAX_ENTITYID; i++) {
		if ((eMng.isAlloc(i)) && (cMng.hasComponent(eMng.entities[i], cid_trans))) {
			glPushMatrix();
				glTranslatef(compTrans.pos[i][0], compTrans.pos[i][1], compTrans.pos[i][2]);
				if(cMng.hasComponent(eMng.entities[i],cid_mat))
					modelMaterial(compMat.diff[i], compMat.ambient[i]);
				else {
					modelMaterial(mat_non_diff, mat_non_diff);
					std::cout << "2222" << std::endl;
				}					
				glutSolidTeapot(0.1);
			glPopMatrix();
		}
	}

	glutSwapBuffers();
}

void idle() {
	if (sceneID != new_sceneID) {
		// scene transform
		if (new_sceneID == MAIN_SCENE) {
			logger->log(new std::string("new scene"));
			for (int i = 0; i < 256; i++) {
				auto eid = eMng.AllocEntity();
				cMng.AssignComp(&eMng, eid, cid_trans);
				cMng.AssignComp(&eMng, eid, cid_mobile);
				cMng.AssignComp(&eMng, eid, cid_mat);
				cMng.AssignComp(&eMng, eid, cid_regen);

				compTrans.pos[eid][0] = ((float)rand() / (float)(RAND_MAX)-0.5f) * 2.0f;
				compTrans.pos[eid][1] = 10.0f * (((float)rand() / (float)(RAND_MAX))-0.5f);
				compTrans.pos[eid][2] = ((float)rand() / (float)(RAND_MAX)-0.5f) * 2.0f;

				compMobile.mass = 1.0f;
				compMobile.force[eid][1] = -0.1f;
				compMobile.vel[eid][0] = compMobile.vel[eid][2] = 0.0f;
				compMobile.vel[eid][1] = -0.10f;

				compMat.diff[eid][0] = compMat.ambient[eid][0] 
					= ((float)rand() / (float)(RAND_MAX));
				compMat.diff[eid][1] = compMat.ambient[eid][1]
					= ((float)rand() / (float)(RAND_MAX));
				compMat.diff[eid][2] = compMat.ambient[eid][2]
					= ((float)rand() / (float)(RAND_MAX));
				compMat.diff[eid][3] = compMat.ambient[eid][3]
					= 1.0f;
				compMat.specular[eid][0] = compMat.specular[eid][1]
					= compMat.specular[eid][2] = compMat.specular[eid][3]
					= 1.0f;
				compMat.shininess[eid][0] = 50.0f;
			}
			// update sceneID
			sceneID = new_sceneID;
		}
		else if (new_sceneID == -1) {
			eMng.FreeAll();
			sceneID = new_sceneID;
			new_sceneID = MAIN_SCENE;
		}
	}
	// deleting entities

	// action
	
	int cur_time = glutGet(GLUT_ELAPSED_TIME);
	auto delta = cur_time - old_time;
	if (delta >= 10) {
		Data_ActionPhysics data;
		data.delta = (float)delta * 0.001f;
		old_time = cur_time;

		data.compMobile = &compMobile;
		data.compTrans = &compTrans;
		actionPhysics.start((void*)&data);
	}

	Data_ActionRegen data_regen;
	data_regen.compMobile = &compMobile;
	data_regen.compTrans = &compTrans;
	data_regen.compRegen = &compRegen;
	actionRegen.start((void*)&data_regen);
}

void draw_timer(int val) {
	glutPostRedisplay();
	glutTimerFunc(7, draw_timer, 0);
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'q':
	case 'Q':
		exit(EXIT_SUCCESS);
		break;
	case 'r':
	case 'R':
		new_sceneID = -1;
		break;
	}
	glutPostRedisplay();
}
void reshape(int w, int h) {
	glViewport(0, 0, w, h);

	std::cout << "Width : " << w << "       height : " << h << std::endl;

	GLfloat widthFactor = (GLfloat)w / (GLfloat)INIT_W;
	GLfloat heightFactor = (GLfloat)h / (GLfloat)INIT_H;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-1.0*widthFactor,1.0*widthFactor,-1.0*heightFactor,1.0*heightFactor,-1.0,1.0);
	gluPerspective(50.0, (GLdouble)w / (GLdouble)h, 0.5, 10.0);
}

class EngineCore
{
	void gl_init(int* argc, char** argv) {
		glutInit(argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
		glutInitWindowSize(INIT_W, INIT_H);
		glutInitWindowPosition(0, 0);
		glutCreateWindow("TESTING");
		glClearColor(0.0, 0.0, 0.0, 1.0);
	}	

	void InitVisibility() {
		glEnable(GL_DEPTH_TEST);
	}
	void InitLight() {

		GLfloat light_specular[] = { 1.0,1.0,1.0,1.0 };
		GLfloat light_diff[] = { 0.8,0.8,0.8,1.0 };
		GLfloat light_ambient[] = { 0.3,0.3,0.3,1.0 };
		GLfloat light_position[] = { -3,2,3.0,0.0 };

		glShadeModel(GL_SMOOTH);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diff);
		glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	}
public:
	void run(int* argc, char** argv) {
		logger = new Logger(true, 128, true, false);

		this->gl_init(argc, argv);

		this->InitVisibility();
		this->InitLight();

		glutDisplayFunc(draw);
		glutIdleFunc(idle);
		glutTimerFunc(0, draw_timer, 0);
		glutKeyboardFunc(keyboard);
		glutReshapeFunc(reshape);

		RegisterComponent();
		logger->log(new std::string("RUN"));

		actionPhysics.eMng = &eMng;
		actionPhysics.cMng = &cMng;
		actionPhysics.required = (1 << cid_trans) | (1 << cid_mobile);

		actionRegen.eMng = &eMng;
		actionRegen.cMng = &cMng;
		actionRegen.required = (1 << cid_trans) | (1 << cid_mobile) | (1<<cid_regen);

		old_time = glutGet(GLUT_ELAPSED_TIME);

		glutMainLoop();
	}
};

