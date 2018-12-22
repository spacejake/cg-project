#pragma once

#include <stdio.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <limits>


#include <glm/glm.hpp>
#include <glm/ext.hpp>


#include <GL/glew.h>
#include <assimp/scene.h>



struct MyMesh {

	GLuint vao;
	GLuint texIndex;
	GLuint uniformBlockIndex;
	int numFaces;
};

class Mesh
{
private:
	std::vector<struct MyMesh> myMeshes;

	std::vector<aiNode*> ai_nodes;

	std::vector<glm::mat4> saved_matrices[30];
	std::vector<float *> matrixStack;
	float current_matrix[16];

	float scaleFactor;

	int type;

	bool isStatic;
	bool isPlayer = false;
	unsigned int sprite = 0;
	unsigned int fps = 30;
	unsigned int frame = 0;
public:

	Mesh();
	void init(std::string path, bool isStatic = false, bool isPlayer= false);
	void Mesh::render(int frame);
	//void render(ProgramWrapperMain& prog);

	void get_bounding_box(const aiScene* scene, aiVector3D* min, aiVector3D* max);
	void get_bounding_box_for_node(const aiScene* scene, const aiNode* nd,
		aiVector3D* min,
		aiVector3D* max);
	float getScaleFactor(void);
	void recursiveNodeProcess(aiNode *);
	void pushMatrix();
	void popMatrix();
	void saveMatrix(int);
	int LoadGLTextures(const aiScene* scene);
};

enum MODE
{
	FPS,
	TPS
};
class Camera

{

private:

	glm::vec3 pos;

	glm::vec2 rot;

	float fov = 45.0f;

	float rotSpeed = 32.0f;

	float walkSpeed = 1.0f;

	clock_t modeDelay;
	clock_t lastConvert;

public:
	enum MODE mode;

	void init(const glm::vec3& pos, const glm::vec2& rot);
	void update(float delta);

	glm::mat4 toViewMatrix();

	glm::mat4 toProjMatrix();



	glm::vec3 getPos();

	void setPos(const glm::vec3& pos);



	glm::vec2 getRot();

	void setRot(const glm::vec2& rot);



	float getFOV();

	void setFOV(float fov);



	float getRotSpeed();

	void setRotSpeed(float rotSpeed);

	void convertMode();

};