#include <iostream>

#include <GL/glut.h>
#include <GL/GL.h>
#include <GL/GLU.h>

#include "Scene.h"
#include "ECS.h"

#include <random>


#define INIT_W 720
#define INIT_H 480

// delta time for 144fps
#define delta (7)

#define N (1)

using namespace std;

Scene scene;
ComponentID phyId;
ComponentID trsld;


void gl_init(int* argc, char** argv) {
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(INIT_W, INIT_H);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("TESTING");
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-1.0, 1.0, -1.0, 1.0, -3.0, 3.0);
	gluPerspective(85.0, 16.0 / 9.0, 0.1, 3.0);
}

void scene_init() {
	for (int i = 0; i < 10; i++) {
		auto eid = scene.AddEntity();
		scene.AssignComponent(eid, scene.physicsID);
		scene.AssignComponent(eid, scene.transformID);

		phyId = scene.physicsID;
		trsld = scene.transformID;
		
		((Component<Transform>*)scene.compMng.components[trsld])->Entities[eid].pos[0] = (float)rand() / (float)(RAND_MAX)-0.5;
		((Component<Transform>*)scene.compMng.components[trsld])->Entities[eid].pos[1] = 0.0;//2.0;
		((Component<Transform>*)scene.compMng.components[trsld])->Entities[eid].pos[2] = (float)rand() / (float)(RAND_MAX)-0.5; 

		((Component<Physics>*)scene.compMng.components[phyId])->Entities[eid].force[1] = -((float)rand() / (float)RAND_MAX) - 0.1;
	}
}

void draw() {
	auto pos = ((Component<Transform>*)scene.compMng.components[trsld]);

	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(-1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glColor3f(1.0, 1.0, 0.0);
	glPushMatrix();
		glRotatef(90.0, 1.0, 0, 0);
		glutSolidTorus(0.1, 0.5, 20, 30);
	glPopMatrix();
	for (int i=0; i < MAX_EntityID; i++){
		if (!scene.entityMng.isAllocated(i)) continue;
		auto position = pos->Entities[i].pos;
		glPushMatrix();
			glTranslatef(position[0], position[1], position[2]);
			glColor3f(1.0, 1.0, 1.0);
			glutWireSphere(0.1, 20, 16);
		glPopMatrix();
	}

	glutSwapBuffers();
}

void keyboard() {

}

void FixedTime(int Value) {
	scene.updatePhysics(delta*0.001f);
	glutPostRedisplay();
	glutTimerFunc(delta, FixedTime, 0);
}

int main(int argc, char** argv) {
	gl_init(&argc,argv);
	scene_init();


	glutDisplayFunc(draw);
	glutTimerFunc(0, FixedTime, 0);

	glutMainLoop();

	return 0;
}