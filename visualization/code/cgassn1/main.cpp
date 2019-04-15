//
//  main.cpp
//  cgassn1
//
//  Created by Dahun Lee on 2018. 10. 1..
//  Copyright © 2018??Dahun Lee. All rights reserved.
//


#include <iostream>
#include <stdlib.h>
#include <time.h>

#include <windows.h>
#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#include "util.h"
#include "sys.h"
#include "enemy.h"
#include "player.h"
#include "gun.h"
#include "wall.h"
#include "map.h"

using namespace std;



//gluint vertexbuffer;
//gluint uvbuffer;
//gluint matrixid;
//
//
//mesh* m_enemy = new mesh();
//mesh* m_gun = new mesh();
//mesh* m_player = new mesh();
//mesh* m_wall = new mesh();
//////
static Camera camera;
////
////static Enemy enemy;
////static Player player;
////static Gun gun;
////static Wall wall;


static Map* gameMap;


void
init(void)
{

	glGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEXPROC)glutGetProcAddress("glGetUniformBlockIndex");
	glUniformBlockBinding = (PFNGLUNIFORMBLOCKBINDINGPROC)glutGetProcAddress("glUniformBlockBinding");
	glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)glutGetProcAddress("glGenVertexArrays");
	glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)glutGetProcAddress("glBindVertexArray");
	glBindBufferRange = (PFNGLBINDBUFFERRANGEPROC)glutGetProcAddress("glBindBufferRange");
	glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)glutGetProcAddress("glDeleteVertexArrays");

	gameMap = new Map();
	gameMap->init();

}

void
display(void)
{
	gameMap->display();
}

void
reshape(int w, int h)
{
	gameMap->reshape(w, h);
}
void
moveObjects()
{
	gameMap->moveObjects();
	glutPostRedisplay();
	glutSwapBuffers();
}


int
main(int argc, char * argv[])
{
	utilInit();

	glutInit(&argc, argv);


	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200, 150);
	glutInitWindowSize(800, 800);
	windowId = glutCreateWindow("assn3");
	//    glutDisplayFunc(renderScene);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glewInit();
	init();

	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(moveObjects);
	glutKeyboardFunc(myKeyboardFunc);
	glutKeyboardUpFunc(myKeyboardUpFunc);
	glutSpecialFunc(mySpecialFunc);
	glutSpecialUpFunc(mySpecialUpFunc);
	glutMouseFunc(myMouseFunc);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutMainLoop();
}
