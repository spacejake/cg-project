#include "util.h"
#include "sys.h"
#include "ShaderUtil.h"
#include "timer.h"
#include "texture.h"


void Timer::init()
{
	//initText2D();

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	// Initialize Shader
	textShader.Load("vsText.glsl", "fsText.glsl");
	// Initialize texture
	Text2DTextureID = loadDDS("cgassn1/resources/Holstein.DDS");

	// Initialize VBO
	glGenBuffers(1, &Text2DVertexBufferID);
	glGenBuffers(1, &Text2DUVBufferID);


	// Initialize Shader
	textShader.Load("vsText.glsl", "fsText.glsl");
	textShader.bind();
	MatrixID = glGetUniformLocation(textShader.getProgram(), "MVP");
	ViewMatrixID = glGetUniformLocation(textShader.getProgram(), "V");
	ModelMatrixID = glGetUniformLocation(textShader.getProgram(), "M");


	// Initialize uniforms' IDs
	Text2DUniformID = glGetUniformLocation(textShader.getProgram(), "myTextureSampler");
}
void Timer::set(glm::vec3 _pos)
{
	pos.x = _pos.x;
	pos.y = _pos.y;
	pos.z = _pos.z;
}
void Timer::setPos(glm::vec3 _pos)
{
	pos.x = _pos.x;
	pos.y = _pos.y;
	pos.z = _pos.z;
}
void Timer::bind()
{
	this->textShader.bind();
	this->textShader.Use();
}
void Timer::unbind()
{
	this->textShader.unbind();
}

void Timer::display(Camera& camera, int frame)
{




	float yrad = glm::radians(camera.getRot().y);
	//computeMatricesFromInputs();
	glm::mat4 ProjectionMatrix = camera.toProjMatrix();
	glm::mat4 ViewMatrix = camera.toViewMatrix();
	//glm::mat4 World = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x + 25.f * glm::sin(yrad), pos.y + 35, pos.z - 25.0f*glm::cos(yrad)));
	glm::mat4 World = glm::translate(glm::mat4(1.0f), glm::vec3(1.0, 1.0, 1.0));
	
	glm::mat4 ModelMatrix = glm::mat4(1.0);
	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * World * ModelMatrix;



	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glUniformMatrix4fv(this->MatrixID, 1, GL_FALSE, &MVP[0][0]);
	//glUniformMatrix4fv(this->ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
	//glUniformMatrix4fv(this->ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

	char text[256] = { 0, };
	sprintf(text, "12341%d\0", 3);
	

	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> UVs;
	int x = 10;
	int y = 500;
	int size = 60;
	unsigned int length = strlen(text);

	for (unsigned int i = 0; i<length; i++) {

		glm::vec2 vertex_up_left = glm::vec2(x + i * size, y + size);
		glm::vec2 vertex_up_right = glm::vec2(x + i * size + size, y + size);
		glm::vec2 vertex_down_right = glm::vec2(x + i * size + size, y);
		glm::vec2 vertex_down_left = glm::vec2(x + i * size, y);

		vertices.push_back(vertex_up_left);
		vertices.push_back(vertex_down_left);
		vertices.push_back(vertex_up_right);

		vertices.push_back(vertex_down_right);
		vertices.push_back(vertex_up_right);
		vertices.push_back(vertex_down_left);

		char character = text[i];
		float uv_x = (character % 16) / 16.0f;
		float uv_y = (character / 16) / 16.0f;

		glm::vec2 uv_up_left = glm::vec2(uv_x, uv_y);
		glm::vec2 uv_up_right = glm::vec2(uv_x + 1.0f / 16.0f, uv_y);
		glm::vec2 uv_down_right = glm::vec2(uv_x + 1.0f / 16.0f, (uv_y + 1.0f / 16.0f));
		glm::vec2 uv_down_left = glm::vec2(uv_x, (uv_y + 1.0f / 16.0f));
		UVs.push_back(uv_up_left);
		UVs.push_back(uv_down_left);
		UVs.push_back(uv_up_right);

		UVs.push_back(uv_down_right);
		UVs.push_back(uv_up_right);
		UVs.push_back(uv_down_left);
	}
	glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);



	// Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Text2DTextureID);
	glBindVertexArray(this->vao);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(Text2DUniformID, 0);
	

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Draw call
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glDisable(GL_BLEND);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}



//void cleanupText2D() {
//
//	// Delete buffers
//	glDeleteBuffers(1, &Text2DVertexBufferID);
//	glDeleteBuffers(1, &Text2DUVBufferID);
//
//	// Delete texture
//	glDeleteTextures(1, &Text2DTextureID);
//
//	// Delete shader
//	glDeleteProgram(textShader.getProgram());
//}