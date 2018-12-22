#pragma once

#include "util.h"
#include "sys.h"
#include "ShaderUtil.h"



class Timer {

private:

	glm::vec3 pos;

	unsigned int vao;
	unsigned int Text2DTextureID;
	unsigned int Text2DVertexBufferID;
	unsigned int Text2DUVBufferID;

	unsigned int Text2DUniformID;

	ShaderUtil textShader;
	unsigned int remainTime;
	

	
	GLuint MatrixID;
	GLuint ViewMatrixID;
	GLuint ModelMatrixID;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	


	
	GLuint LightID;

public:
	void init();
	void set(glm::vec3);
	void setPos(glm::vec3);
	void bind();
	void unbind();
	void display(Camera& camera, int frame);

};