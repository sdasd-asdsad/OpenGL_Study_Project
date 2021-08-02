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

#include "BaseAction.h"
#include "ActionPhyics.h"

#include "collisionCalc.h"

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
////////////////////////

// ACTION CONTAINER
std::vector<BaseAction> actions;
ActionPhyics actionPhysics;


// Cell Manager
CellMng cellMng(CEEL_PARTITION);


// timer
int old_time;

GLfloat mat_sun_diff[] = { 1.0,1.0,1.0,1.0 };
GLfloat mat_non_diff[] = { 1.0,1.0,1.0,1.0 };
GLfloat mat_sun_ambient[] = { 0.93,0.21,0.06,1.0 };
GLfloat mat_specular[] = { 1.0,1.0,1.0,1.0 };
GLfloat mat_shininess[] = { 50.0 };

GLfloat wallVert[][3] = {
	{0.0,0.0,0.0},
	{MAX_DIST,0.0,0.0},
	{MAX_DIST,0.0,MAX_DIST},
	{0.0,0.0,MAX_DIST},
	{0.0,MAX_DIST,0.0},
	{MAX_DIST,MAX_DIST,0.0},
	{MAX_DIST,MAX_DIST,MAX_DIST},
	{0.0,MAX_DIST,MAX_DIST}
};

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
}

void draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	auto cam_x = view_rad * sin(deg2rad(view_lat)) * cos(deg2rad(view_long))+ MAX_DIST / 2.0f;
	auto cam_z = view_rad * sin(deg2rad(view_lat)) * sin(deg2rad(view_long)) + MAX_DIST / 2.0f;
	auto cam_y = view_rad * cos(deg2rad(view_lat)) + MAX_DIST / 2.0f;

	gluLookAt(cam_x, cam_y, cam_z,
		MAX_DIST/2.0f, MAX_DIST / 2.0f, MAX_DIST / 2.0f,
		0.0, 1.0, 0.0);

	modelMaterial(mat_sun_diff, mat_sun_ambient);
	glPushMatrix();
	glTranslatef(MAX_DIST / 2.0f, MAX_DIST / 2.0f, MAX_DIST / 2.0f);
	glutWireCube(MAX_DIST);
	glPopMatrix();

	for (int i = 0; i < MAX_ENTITYID; i++) {
		if ((eMng.isAlloc(i)) && (cMng.hasComponent(eMng.entities[i], cid_trans))) {
			glPushMatrix();
				glTranslatef(compTrans.pos[i][0], compTrans.pos[i][1], compTrans.pos[i][2]);
				if(cMng.hasComponent(eMng.entities[i],cid_mat))
					modelMaterial(compMat.diff[i], compMat.ambient[i]);
				else {
					modelMaterial(mat_non_diff, mat_non_diff);
				}					
				glutSolidSphere(SIZE_PARTICLE/2.0f, 20, 40);
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
			cellMng.initCells();
			for (int i = 0; i < NUM_PARTICLE; i++) {
				auto eid = eMng.AllocEntity();
				cMng.AssignComp(&eMng, eid, cid_trans);
				cMng.AssignComp(&eMng, eid, cid_mobile);
				cMng.AssignComp(&eMng, eid, cid_mat);

				compTrans.pos[eid][0] = 
					(MAX_DIST-2.0f*SIZE_PARTICLE)*((float)rand() / (float)(RAND_MAX))
					+ SIZE_PARTICLE;
				compTrans.pos[eid][1] =
					(MAX_DIST - 2.0f * SIZE_PARTICLE) * ((float)rand() / (float)(RAND_MAX))
					+ SIZE_PARTICLE;
				compTrans.pos[eid][2] =
					(MAX_DIST - 2.0f * SIZE_PARTICLE) * ((float)rand() / (float)(RAND_MAX))
					+ SIZE_PARTICLE;

				auto cellNum = cellMng.getCellNum(compTrans.pos[eid][0],
					compTrans.pos[eid][1], compTrans.pos[eid][2]);
				cellMng.addEntity(eid, cellNum);

				compMobile.mass[eid] = 1.0f;
				compMobile.force[eid][0] = compMobile.force[eid][1]
					= compMobile.force[eid][2] = 0.0f;
				compMobile.vel[eid][1] = 2.0f * ((float)rand() / (float)(RAND_MAX)) * MAX_SPEED_PARTICLE - MAX_SPEED_PARTICLE;
				compMobile.vel[eid][2] = 2.0f * ((float)rand() / (float)(RAND_MAX)) * MAX_SPEED_PARTICLE - MAX_SPEED_PARTICLE;
				compMobile.vel[eid][0] = 2.0f * ((float)rand() / (float)(RAND_MAX)) * MAX_SPEED_PARTICLE - MAX_SPEED_PARTICLE;
				compMobile.collisionChecked[eid] = true;

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
		data.cellMng = &cellMng;
		actionPhysics.start((void*)&data);
	}
	for (int i = 0; i < MAX_ENTITYID;i++) {
		if (eMng.isAlloc(i)&&cMng.hasComponent(eMng.entities[i], cid_mobile)) {
			compMobile.collisionChecked[i] = false;
		}
	}
}

void draw_timer(int val) {
	glutPostRedisplay();
	glutTimerFunc(0, draw_timer, 0);
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

bool isMoving = false;
int old_x = -1;
int old_y = -1;

void mouseClick(GLint button,GLint state, int x, int y) {
	if (button==GLUT_LEFT_BUTTON && state==GLUT_DOWN) {
		if (!isMoving) {
			old_x = x;
			old_y = y;
			isMoving = true;
		}
		else {
			std::cout << "??" << std::endl;
		}
	}
	else if((button == GLUT_LEFT_BUTTON && state == GLUT_UP)){
		isMoving = false;
	}
	std::cout << "MOVING : " << isMoving << std::endl;
}

void mouseMotion(int x, int y) {
	if (!isMoving)
		return;
	std::cout << "X : " << x - old_x << " - Y : " << y - old_y << std::endl;
	view_long += (GLint)((float)(x - old_x) * view_sensitivity);
	view_lat -= (GLint)((float)(y - old_y) * view_sensitivity);
	if (view_long < 0) {
		view_long += 7200;
	}
	if (view_lat < 10) {
		view_lat = 10;
	}
	else if (view_lat > 170) {
		view_lat = 170;
	}
	view_long %= 360;
	old_x = x;
	old_y = y;
	std::cout << "LONG : " << view_long << " - LAT : " << view_lat << std::endl;
}

void mouseWheel(int button, int dir, int x, int y) {
	if (dir > 0)
	{
		// Zoom in
		view_rad -= 0.2f;
	}
	else
	{
		// Zoom out
		view_rad += 0.2f;
	}
	return;
}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);

	std::cout << "Width : " << w << "       height : " << h << std::endl;

	GLfloat widthFactor = (GLfloat)w / (GLfloat)INIT_W;
	GLfloat heightFactor = (GLfloat)h / (GLfloat)INIT_H;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-1.0*widthFactor,1.0*widthFactor,-1.0*heightFactor,1.0*heightFactor,-1.0,1.0);
	gluPerspective(90.0, (GLdouble)w / (GLdouble)h, 0.5, 20.0);
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
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);
	}
	void InitLight() {

		GLfloat light_specular[] = { 1.0,1.0,1.0,1.0 };
		GLfloat light_diff[] = { 1.0,1.0,1.0,1.0 };
		GLfloat light_ambient[] = { 0.9,0.9,0.9,1.0 };
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
		glutMouseFunc(mouseClick);
		glutMotionFunc(mouseMotion);
		glutMouseWheelFunc(mouseWheel);
		glutReshapeFunc(reshape);

		RegisterComponent();
		logger->log(new std::string("RUN"));

		actionPhysics.eMng = &eMng;
		actionPhysics.cMng = &cMng;
		actionPhysics.required = (1 << cid_trans) | (1 << cid_mobile);

		old_time = glutGet(GLUT_ELAPSED_TIME);

		glutMainLoop();
	}
};

