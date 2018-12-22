//
//  util.h
//  cgassn1
//
//  Created by Dahun Lee on 2018. 10. 1..
//  Copyright © 2018??Dahun Lee. All rights reserved.
//

#ifndef util_h
#define util_h

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#include <GLUT/GLUT.h>
#else
#include <windows.h>
#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <time.h>
#include <list>
#include "ShaderUtil.h"
#include "sys.h"



#include <iostream>


#define KEYBOARD_BUFFER_SIZE 256
#define GLOBAL_GRID_LENGTH 50
#define MOUSE_BUFFER_SIZE 3

#define COL_LEFT (1<<0)
#define COL_UP (1<<1)
#define COL_RIGHT (1<<2)
#define COL_DOWN (1<<3)

#define BUFFER_OFFSET( offset ) ((GLvoid*)(offset))

enum Direction { LEFT, UP, RIGHT, DOWN };
enum Status { ALIVE, KILLED };
enum ItemType { ONE, TWO};

extern bool
keyboardBuffer [KEYBOARD_BUFFER_SIZE];


extern bool
specialKeyBuffer [KEYBOARD_BUFFER_SIZE];

extern bool
mouseBuffer[MOUSE_BUFFER_SIZE];

extern unsigned mouseBefore;

typedef struct
{
    float x;
    float y;
}position;

// This is for a shader uniform block
struct MyMaterial {

	float diffuse[4];
	float ambient[4];
	float specular[4];
	float emissive[4];
	float shininess;
	int texCount;
};

extern ShaderUtil shaderUtil;


extern GLuint projID;
extern GLuint viewID;
extern GLuint modelID;
extern GLuint materialUniLoc;
extern GLuint texUnit;
extern GLuint viewPosID;
extern GLuint light_ID;
extern GLuint light_ID2;

extern GLuint ani;


extern
std::map<std::string, GLuint> textureIdMap;

extern GLuint vertexbuffer;
extern GLuint uvbuffer;

extern ShaderUtil shaderWallUtil;
extern GLuint wallProgramID;
extern GLuint MatrixID;
extern GLuint ViewMatrixID;
extern GLuint ModelMatrixID;
extern GLuint ModelView3x3MatrixID;



extern GLuint DiffuseTexture;
extern GLuint NormalTexture;
extern GLuint SpecularTexture;

extern GLuint DiffuseTextureID;
extern GLuint NormalTextureID;
extern GLuint SpecularTextureID;

extern GLuint vertexbufferWall;
extern GLuint uvbufferWall;
extern GLuint normalbufferWall;
extern GLuint tangentbufferWall;
extern GLuint bitangentbufferWall;
extern GLuint elementbufferWall;
extern GLuint LightID;
extern GLuint LightID2;

extern std::vector<glm::vec3> vertices;
extern std::vector<glm::vec2> uvs;
extern std::vector<glm::vec3> normals;
//bool res = loadOBJ("cylinder.obj", vertices, uvs, normals);

extern std::vector<glm::vec3> tangents;
extern std::vector<glm::vec3> bitangents;

extern std::vector<unsigned short> indices;
extern std::vector<glm::vec3> indexed_vertices;
extern std::vector<glm::vec2> indexed_uvs;
extern std::vector<glm::vec3> indexed_normals;
extern std::vector<glm::vec3> indexed_tangents;
extern std::vector<glm::vec3> indexed_bitangents;

extern unsigned int
windowId;

extern bool
gameClear;

void
utilInit ();

void
myKeyboardFunc (unsigned char, int, int);

void
myKeyboardUpFunc (unsigned char, int, int);

void
mySpecialFunc (int key, int x, int y);

void
mySpecialUpFunc (int key, int x, int y);

void
myMouseFunc(int, int, int, int);

void
myMouseUpFunc(int, int, int, int);

unsigned int
CheckCollision(position pos1, position pos2, float len1, float len2);

unsigned int
CheckCollisionWeapon (position , position );

float
calDistance (float x1, float y1, float x2, float y2);

void
renderbitmap(float x, float y,float z, void*font, char* string);


int
pos2idx(float);

float
idx2pos(int);

std::vector<std::string> split_string(std::string str);


void set_float4(float f[4], float a, float b, float c, float d);

void color4_to_float4(const aiColor4D *c, float f[4]);

void computeTangentBasis(
	std::vector<glm::vec3> & vertices, std::vector<glm::vec2> & uvs, std::vector<glm::vec3> & normals,
	std::vector<glm::vec3> & tangents, std::vector<glm::vec3> & bitangents
);

void indexVBO(
	std::vector<glm::vec3> & in_vertices,
	std::vector<glm::vec2> & in_uvs,
	std::vector<glm::vec3> & in_normals,

	std::vector<unsigned short> & out_indices,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals
);

void indexVBO_TBN(
	std::vector<glm::vec3> & in_vertices,
	std::vector<glm::vec2> & in_uvs,
	std::vector<glm::vec3> & in_normals,
	std::vector<glm::vec3> & in_tangents,
	std::vector<glm::vec3> & in_bitangents,

	std::vector<unsigned short> & out_indices,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals,
	std::vector<glm::vec3> & out_tangents,
	std::vector<glm::vec3> & out_bitangents
);

bool loadOBJ(
	const char * path,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals
);

#endif /* util_h */


