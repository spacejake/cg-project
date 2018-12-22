#include "wall.h"
#include "util.h"


Wall::Wall()
{

}

Wall::Wall(float x, float y)
{
	glm::vec3 pos = glm::vec3(x, 0, y);
	glm::vec2 dir = glm::vec2(0, 0);

	setPos(pos);
	setDir(dir);
}

Wall::Wall(float x, float y, float z)
{
	glm::vec3 pos = glm::vec3(x, z, y);
	glm::vec2 dir = glm::vec2(0, 0);

	setPos(pos);
	setDir(dir);
}

void Wall::init(const glm::vec3& pos, const glm::vec2& dir)
{
	this->setPos(pos);
	this->setDir(dir);
}


void Wall::update(void)
{

}

void Wall::display(Mesh* mesh, Camera& camera, int frame)
{
	glm::mat4 Projection = camera.toProjMatrix();
	glm::mat4 View = camera.toViewMatrix();
	float scaleFactor = 50.0f;//mesh->getScaleFactor();
	glm::mat4 World = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, pos.z));
	glm::mat4 Model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 1 * scaleFactor, 0))
		*glm::rotate(glm::mat4(1.0f), glm::radians(180.f), glm::vec3(0.0f, 0.0f, 1.0f))
		*glm::scale(glm::mat4(1.0f), glm::vec3(scaleFactor, scaleFactor, scaleFactor));

	glm::mat4 mw = World * Model;
	glm::mat4 m3x3 = glm::mat3(Model);
	glm::mat4 mvp = Projection * View *World* Model;




	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &View[0][0]); //delete later
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &mw[0][0]);
	glUniformMatrix3fv(ModelView3x3MatrixID, 1, GL_FALSE, &m3x3[0][0]);
	//glUniformMatrix4fv(ani, 1, GL_FALSE, &glm::mat4(1.0f)[0][0]);

	//glm::vec3 lightPos = glm::vec3(0, 0, 4);
	//glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

	// Bind our diffuse texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, DiffuseTexture);
	// Set our "DiffuseTextureSampler" sampler to use Texture Unit 0
	glUniform1i(DiffuseTextureID, 0);

	// Bind our normal texture in Texture Unit 1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, NormalTexture);
	// Set our "NormalTextureSampler" sampler to use Texture Unit 1
	glUniform1i(NormalTextureID, 1);

	// Bind our specular texture in Texture Unit 2
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, SpecularTexture);
	// Set our "SpecularTextureSampler" sampler to use Texture Unit 2
	glUniform1i(SpecularTextureID, 2);


	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferWall);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbufferWall);
	glVertexAttribPointer(
		1,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// 3rd attribute buffer : normals
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalbufferWall);
	glVertexAttribPointer(
		2,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// 4th attribute buffer : tangents
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, tangentbufferWall);
	glVertexAttribPointer(
		3,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// 5th attribute buffer : bitangents
	glEnableVertexAttribArray(4);
	glBindBuffer(GL_ARRAY_BUFFER, bitangentbufferWall);
	glVertexAttribPointer(
		4,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbufferWall);
	

	glDrawElements(
		GL_TRIANGLES,      // mode
		indices.size(),    // count
		GL_UNSIGNED_SHORT, // type
		(void*)0           // element array buffer offset

	);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
}



glm::vec3 Wall::getPos()

{

	return this->pos;

}



void Wall::setPos(const glm::vec3& pos)

{

	this->pos = pos;

}



glm::vec2 Wall::getDir()

{

	return this->dir;

}



void Wall::setDir(const glm::vec2& dir)
{
	this->dir = dir;
}
