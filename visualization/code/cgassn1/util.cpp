//
//  util.cpp
//  cgassn1
//
//  Created by Dahun Lee on 2018. 10. 1..
//  Copyright © 2018??Dahun Lee. All rights reserved.
//

#include "util.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include <cmath>


#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#include <GLUT/GLUT.h>
#else
#include <windows.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#endif


ShaderUtil shaderUtil;


GLuint projID;
GLuint viewID;
GLuint modelID;
GLuint materialUniLoc;
GLuint texUnit;
GLuint viewPosID;
GLuint light_ID;
GLuint light_ID2;

GLuint ani;

GLuint vertexbuffer;
GLuint uvbuffer;


std::map<std::string, GLuint> textureIdMap;

// for vsWall
ShaderUtil shaderWallUtil;
GLuint wallProgramID;
GLuint MatrixID;
GLuint ViewMatrixID;
GLuint ModelMatrixID;
GLuint ModelView3x3MatrixID;

GLuint DiffuseTexture;
GLuint NormalTexture;
GLuint SpecularTexture;

GLuint DiffuseTextureID;
GLuint NormalTextureID;
GLuint SpecularTextureID;

GLuint vertexbufferWall;
GLuint uvbufferWall;
GLuint normalbufferWall;
GLuint tangentbufferWall;
GLuint bitangentbufferWall;
GLuint elementbufferWall;
GLuint LightID;
GLuint LightID2;

std::vector<glm::vec3> vertices;
std::vector<glm::vec2> uvs;
std::vector<glm::vec3> normals;
//bool res = loadOBJ("cylinder.obj", vertices, uvs, normals);

std::vector<glm::vec3> tangents;
std::vector<glm::vec3> bitangents;

std::vector<unsigned short> indices;
std::vector<glm::vec3> indexed_vertices;
std::vector<glm::vec2> indexed_uvs;
std::vector<glm::vec3> indexed_normals;
std::vector<glm::vec3> indexed_tangents;
std::vector<glm::vec3> indexed_bitangents;

unsigned int
windowId;

bool
gameClear;

bool
keyboardBuffer [KEYBOARD_BUFFER_SIZE];


bool
specialKeyBuffer [KEYBOARD_BUFFER_SIZE];

bool
mouseBuffer[MOUSE_BUFFER_SIZE];

unsigned mouseBefore;

void
utilInit ()
{
	srand(time(NULL));
    int i;
    
    for (i=0 ; i < KEYBOARD_BUFFER_SIZE ; i++)
    {
        keyboardBuffer[i] = false;
        specialKeyBuffer[i] = false;
    }
    gameClear = false;
	mouseBefore = GLUT_UP;
}
//
void
myKeyboardFunc (unsigned char key, int x, int y)
{
    keyboardBuffer[key] = true;
}

void
myKeyboardUpFunc (unsigned char key, int x, int y)
{
    keyboardBuffer[key] = false;
}


void
mySpecialFunc (int key, int x, int y)
{
    specialKeyBuffer[key] = true;
}

void
mySpecialUpFunc (int key, int x, int y)
{
    specialKeyBuffer[key] = false;
}

void
myMouseFunc(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		mouseBuffer[button] = true;
		mouseBefore = GLUT_DOWN;
	}

	else
	{
		mouseBuffer[button] = false;
		mouseBefore = GLUT_UP;
	}
}

void
myMouseUpFunc(int, int, int, int)
{

}

unsigned int
CheckCollision (position pos1, position pos2, float len1, float len2)
{
    unsigned int ret = 0;
    
    
    // check right side
    bool CollisionX = pos1.x + GLOBAL_GRID_LENGTH >= pos2.x && pos2.x >= pos1.x;
    bool CollisionY = pos1.y + GLOBAL_GRID_LENGTH > pos2.y && pos2.y + GLOBAL_GRID_LENGTH > pos1.y;
    
    if (CollisionX && CollisionY) ret |= COL_RIGHT;
    
    // check left side
    CollisionX = pos1.x <= pos2.x + GLOBAL_GRID_LENGTH && pos2.x <= pos1.x;
    CollisionY = pos1.y + GLOBAL_GRID_LENGTH > pos2.y && pos2.y + GLOBAL_GRID_LENGTH > pos1.y;
    
    if (CollisionX && CollisionY) ret |= COL_LEFT;
    
    // check up side
    CollisionX = pos1.x + GLOBAL_GRID_LENGTH > pos2.x  && pos2.x + GLOBAL_GRID_LENGTH > pos1.x;
    CollisionY = pos1.y + GLOBAL_GRID_LENGTH >= pos2.y && pos2.y >= pos1.y;
    
    if (CollisionX && CollisionY) ret |= COL_UP;
    
    // check down side
    CollisionX = pos1.x + GLOBAL_GRID_LENGTH > pos2.x  && pos2.x + GLOBAL_GRID_LENGTH > pos1.x;
    CollisionY = pos1.y  <= pos2.y + GLOBAL_GRID_LENGTH && pos2.y <= pos1.y;
    
    if (CollisionX && CollisionY) ret |= COL_DOWN;
    
    return ret;
}

unsigned int
CheckCollisionWeapon (position pos1, position pos2)
{
    unsigned int ret = 0;
    
    
    // check right side
    bool CollisionX = pos1.x + GLOBAL_GRID_LENGTH/5 >= pos2.x && pos2.x >= pos1.x;
    bool CollisionY = pos1.y + GLOBAL_GRID_LENGTH/5 > pos2.y && pos2.y + GLOBAL_GRID_LENGTH > pos1.y;
    
    if (CollisionX && CollisionY) ret |= COL_RIGHT;
    
    // check left side
    CollisionX = pos1.x <= pos2.x + GLOBAL_GRID_LENGTH/5 && pos2.x <= pos1.x;
    CollisionY = pos1.y + GLOBAL_GRID_LENGTH/5 > pos2.y && pos2.y + GLOBAL_GRID_LENGTH > pos1.y;
    
    if (CollisionX && CollisionY) ret |= COL_LEFT;
    
    // check up side
    CollisionX = pos1.x + GLOBAL_GRID_LENGTH/5 > pos2.x  && pos2.x + GLOBAL_GRID_LENGTH > pos1.x;
    CollisionY = pos1.y + GLOBAL_GRID_LENGTH/5 >= pos2.y && pos2.y >= pos1.y;
    
    if (CollisionX && CollisionY) ret |= COL_UP;
    
    // check down side
    CollisionX = pos1.x + GLOBAL_GRID_LENGTH/5 > pos2.x  && pos2.x + GLOBAL_GRID_LENGTH > pos1.x;
    CollisionY = pos1.y  <= pos2.y + GLOBAL_GRID_LENGTH && pos2.y <= pos1.y;
    
    if (CollisionX && CollisionY) ret |= COL_DOWN;
    
    return ret;
}


float
calDistance (float x1, float y1, float x2, float y2)
{
    return (sqrt (pow(x2-x1, 2) + pow(y2-y1, 2)));
}



void
renderbitmap(float x, float y, float z, void*font, char* string) {
	char *c;
	glRasterPos3f(x, y,z);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}


int
pos2idx(float x)
{
	return (int)(x / 50.f);
}

float
idx2pos(int x)
{
	return (float)(x * 50.f + 25.0f);
}
std::vector<std::string>
split_string(std::string str) {
	using namespace std;

	string buf;
	stringstream ss(str);

	vector<string> tokens;

	while (ss >> buf)
		tokens.push_back(buf);

	return tokens;

}


void set_float4(float f[4], float a, float b, float c, float d)
{
	f[0] = a;
	f[1] = b;
	f[2] = c;
	f[3] = d;
}


void color4_to_float4(const aiColor4D *c, float f[4])
{
	f[0] = c->r;
	f[1] = c->g;
	f[2] = c->b;
	f[3] = c->a;
}

void computeTangentBasis(
	// inputs
	std::vector<glm::vec3> & vertices,
	std::vector<glm::vec2> & uvs,
	std::vector<glm::vec3> & normals,
	// outputs
	std::vector<glm::vec3> & tangents,
	std::vector<glm::vec3> & bitangents
) {

	for (unsigned int i = 0; i<vertices.size(); i += 3) {

		// Shortcuts for vertices
		glm::vec3 & v0 = vertices[i + 0];
		glm::vec3 & v1 = vertices[i + 1];
		glm::vec3 & v2 = vertices[i + 2];

		// Shortcuts for UVs
		glm::vec2 & uv0 = uvs[i + 0];
		glm::vec2 & uv1 = uvs[i + 1];
		glm::vec2 & uv2 = uvs[i + 2];

		// Edges of the triangle : postion delta
		glm::vec3 deltaPos1 = v1 - v0;
		glm::vec3 deltaPos2 = v2 - v0;

		// UV delta
		glm::vec2 deltaUV1 = uv1 - uv0;
		glm::vec2 deltaUV2 = uv2 - uv0;

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r;
		glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x)*r;

		// Set the same tangent for all three vertices of the triangle.
		// They will be merged later, in vboindexer.cpp
		tangents.push_back(tangent);
		tangents.push_back(tangent);
		tangents.push_back(tangent);

		// Same thing for binormals
		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);

	}

	// See "Going Further"
	for (unsigned int i = 0; i<vertices.size(); i += 1)
	{
		glm::vec3 & n = normals[i];
		glm::vec3 & t = tangents[i];
		glm::vec3 & b = bitangents[i];

		// Gram-Schmidt orthogonalize
		t = glm::normalize(t - n * glm::dot(n, t));

		// Calculate handedness
		if (glm::dot(glm::cross(n, t), b) < 0.0f) {
			t = t * -1.0f;
		}

	}


}

// Returns true iif v1 can be considered equal to v2
bool is_near(float v1, float v2) {
	return fabs(v1 - v2) < 0.01f;
}

// Searches through all already-exported vertices
// for a similar one.
// Similar = same position + same UVs + same normal
bool getSimilarVertexIndex(
	glm::vec3 & in_vertex,
	glm::vec2 & in_uv,
	glm::vec3 & in_normal,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals,
	unsigned short & result
) {
	// Lame linear search
	for (unsigned int i = 0; i<out_vertices.size(); i++) {
		if (
			is_near(in_vertex.x, out_vertices[i].x) &&
			is_near(in_vertex.y, out_vertices[i].y) &&
			is_near(in_vertex.z, out_vertices[i].z) &&
			is_near(in_uv.x, out_uvs[i].x) &&
			is_near(in_uv.y, out_uvs[i].y) &&
			is_near(in_normal.x, out_normals[i].x) &&
			is_near(in_normal.y, out_normals[i].y) &&
			is_near(in_normal.z, out_normals[i].z)
			) {
			result = i;
			return true;
		}
	}
	// No other vertex could be used instead.
	// Looks like we'll have to add it to the VBO.
	return false;
}

void indexVBO_slow(
	std::vector<glm::vec3> & in_vertices,
	std::vector<glm::vec2> & in_uvs,
	std::vector<glm::vec3> & in_normals,

	std::vector<unsigned short> & out_indices,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals
) {
	// For each input vertex
	for (unsigned int i = 0; i<in_vertices.size(); i++) {

		// Try to find a similar vertex in out_XXXX
		unsigned short index;
		bool found = getSimilarVertexIndex(in_vertices[i], in_uvs[i], in_normals[i], out_vertices, out_uvs, out_normals, index);

		if (found) { // A similar vertex is already in the VBO, use it instead !
			out_indices.push_back(index);
		}
		else { // If not, it needs to be added in the output data.
			out_vertices.push_back(in_vertices[i]);
			out_uvs.push_back(in_uvs[i]);
			out_normals.push_back(in_normals[i]);
			out_indices.push_back((unsigned short)out_vertices.size() - 1);
		}
	}
}

struct PackedVertex {
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;
	bool operator<(const PackedVertex that) const {
		return memcmp((void*)this, (void*)&that, sizeof(PackedVertex))>0;
	};
};

bool getSimilarVertexIndex_fast(
	PackedVertex & packed,
	std::map<PackedVertex, unsigned short> & VertexToOutIndex,
	unsigned short & result
) {
	std::map<PackedVertex, unsigned short>::iterator it = VertexToOutIndex.find(packed);
	if (it == VertexToOutIndex.end()) {
		return false;
	}
	else {
		result = it->second;
		return true;
	}
}

void indexVBO(
	std::vector<glm::vec3> & in_vertices,
	std::vector<glm::vec2> & in_uvs,
	std::vector<glm::vec3> & in_normals,

	std::vector<unsigned short> & out_indices,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals
) {
	std::map<PackedVertex, unsigned short> VertexToOutIndex;

	// For each input vertex
	for (unsigned int i = 0; i<in_vertices.size(); i++) {

		PackedVertex packed = { in_vertices[i], in_uvs[i], in_normals[i] };


		// Try to find a similar vertex in out_XXXX
		unsigned short index;
		bool found = getSimilarVertexIndex_fast(packed, VertexToOutIndex, index);

		if (found) { // A similar vertex is already in the VBO, use it instead !
			out_indices.push_back(index);
		}
		else { // If not, it needs to be added in the output data.
			out_vertices.push_back(in_vertices[i]);
			out_uvs.push_back(in_uvs[i]);
			out_normals.push_back(in_normals[i]);
			unsigned short newindex = (unsigned short)out_vertices.size() - 1;
			out_indices.push_back(newindex);
			VertexToOutIndex[packed] = newindex;
		}
	}
}

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
) {
	// For each input vertex
	for (unsigned int i = 0; i<in_vertices.size(); i++) {

		// Try to find a similar vertex in out_XXXX
		unsigned short index;
		bool found = getSimilarVertexIndex(in_vertices[i], in_uvs[i], in_normals[i], out_vertices, out_uvs, out_normals, index);

		if (found) { // A similar vertex is already in the VBO, use it instead !
			out_indices.push_back(index);

			// Average the tangents and the bitangents
			out_tangents[index] += in_tangents[i];
			out_bitangents[index] += in_bitangents[i];
		}
		else { // If not, it needs to be added in the output data.
			out_vertices.push_back(in_vertices[i]);
			out_uvs.push_back(in_uvs[i]);
			out_normals.push_back(in_normals[i]);
			out_tangents.push_back(in_tangents[i]);
			out_bitangents.push_back(in_bitangents[i]);
			out_indices.push_back((unsigned short)out_vertices.size() - 1);
		}
	}
}

bool loadOBJ(
	const char * path,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals
) {
	printf("Loading OBJ file %s...\n", path);

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;


	FILE * file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
		getchar();
		return false;
	}

	while (1) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

				   // else : parse lineHeader

		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				fclose(file);
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		else {
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// For each vertex of each triangle
	for (unsigned int i = 0; i<vertexIndices.size(); i++) {

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];

		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs.push_back(uv);
		out_normals.push_back(normal);

	}
	fclose(file);
	return true;
}