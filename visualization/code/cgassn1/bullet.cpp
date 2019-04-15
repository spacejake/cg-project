#include "bullet.h"
#include "util.h"


Bullet::Bullet(const glm::vec3& pos, const glm::vec2& dir)
{
	this->setPos(pos);
	this->setDir(dir);
	this->status = ALIVE;
	creationTime = clock();
	lifeTime = CLOCKS_PER_SEC * 3;
}

Bullet::Bullet(const glm::vec3& pos, const glm::vec2& dir, enum Direction _viewDir)
{
	float yrad = glm::radians(dir.y);
	glm::vec3& bPos = glm::vec3(pos.x + 30.f * glm::sin(yrad), 0, pos.z - 30.f * glm::cos(yrad));
	this->setPos(bPos);
	this->setDir(dir);
	this->status = ALIVE;
	creationTime = clock();
	lifeTime = CLOCKS_PER_SEC * 3;
	viewDir = _viewDir;
}

void Bullet::init(const glm::vec3& pos, const glm::vec2& dir)
{
	this->setPos(pos);
	this->setDir(dir);
}


void Bullet::update()
{
	float yrad = glm::radians(dir.y);

	float sp = this->walkSpeed;
	pos.x += sp * glm::sin(yrad);
	pos.z -= sp * glm::cos(yrad);

	if (creationTime + lifeTime < clock())
	{
		status = KILLED;
	}
}


void Bullet::display(Mesh* mesh, Camera& camera, int frame)
{
	float yrad = glm::radians(dir.y);

	glm::mat4 Projection = camera.toProjMatrix();
	glm::mat4 View = camera.toViewMatrix();
	float scaleFactor = mesh->getScaleFactor();
	glm::mat4 World = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x /*+ 30.f * glm::sin(yrad)*/, pos.y+40, pos.z /*- 30.0f*glm::cos(yrad)*/));
	glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(scaleFactor/10, scaleFactor/10, scaleFactor/10));

	glm::mat4 mw = World * Model;

	glUniformMatrix4fv(projID, 1, GL_FALSE, &Projection[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &mw[0][0]);

	mesh->render(frame);
}



glm::vec3 Bullet::getPos()

{
	return this->pos;
}



void Bullet::setPos(const glm::vec3& pos)

{
	this->pos = pos;
}



glm::vec2 Bullet::getDir()

{

	return this->dir;

}



void Bullet::setDir(const glm::vec2& dir)
{
	this->dir = dir;
}

