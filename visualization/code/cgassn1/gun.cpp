#include "gun.h"
#include "util.h"



void Gun::init(const glm::vec3& pos, const glm::vec2& dir)
{
	this->setPos(pos);
	this->setDir(dir);
	bangDelay = CLOCKS_PER_SEC / 2;
	lastbang = 0;
}


void Gun::update(void)
{
	if (lastbang + bangDelay < clock())
	{
		lastbang = 0;
	}
}


void Gun::display(Mesh* mesh, Camera& camera, int frame)
{
	float yrad = glm::radians(dir.y);
	glm::mat4 Projection = camera.toProjMatrix();
	glm::mat4 View = camera.toViewMatrix();
	float scaleFactor = mesh->getScaleFactor();
	glm::mat4 World = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x + 25.f * glm::sin(yrad), pos.y+35, pos.z - 25.0f*glm::cos(yrad)));
	glm::mat4 Model = glm::rotate(glm::mat4(1.0f), glm::radians(-camera.getRot().y ), glm::vec3(0.0f, 1.0f, 0.0f))
		* glm::scale(glm::mat4(1.0f), glm::vec3(scaleFactor/5, scaleFactor/5, scaleFactor/5));

	glm::mat4 mw = World * Model;

	glUniformMatrix4fv(projID, 1, GL_FALSE, &Projection[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &mw[0][0]);
	mesh->render(frame);
}



glm::vec3 Gun::getPos()

{
	return this->pos;
}



void Gun::setPos(const glm::vec3& pos)

{
	this->pos = pos;
}



glm::vec2 Gun::getDir()
{
	return this->dir;
}

void Gun::setDir(const glm::vec2& dir)
{
	this->dir = dir;
}

void Gun::bang()
{
	lastbang = clock();
}

bool Gun::canShoot()
{
	return lastbang == 0;
}