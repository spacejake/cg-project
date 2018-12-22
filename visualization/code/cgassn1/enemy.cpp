#include "enemy.h"
#include "util.h"



Enemy::Enemy(float x, float y, enum Direction vd)
{
	glm::vec3 pos = glm::vec3(x, 0, y);
	glm::vec2 dir = glm::vec2(0, 0);

	setPos(pos);
	setDir(dir);

	status = ALIVE;
	canGo = true;
	viewDir = vd;
}

void Enemy::init(const glm::vec3& pos, const glm::vec2& dir)
{
	this->setPos(pos);
	this->setDir(dir);
	status = ALIVE;
}


void Enemy::update(glm::vec3 player)
{
	if (status == ALIVE)
	{
		if (calDistance(player.x, player.z, pos.x, pos.z) < 250) // some condition to trace player
		{
			if (player.x >= pos.x)
			{
				if (canGo)
				{
					pos.x += 2 * walkSpeed;
					viewDir = RIGHT;
				}
			}
			else {
				if (canGo)
				{
					pos.x -=  2 * walkSpeed;
					viewDir = LEFT;
				}
			}

			if (player.z >= pos.z)
			{
				if (canGo)
				{
					pos.z += 2 * walkSpeed;
					if (abs(player.x - pos.x) < abs(player.z - pos.z)) {
						viewDir = UP;

					}
				}
			}
			else
			{
				if (canGo)
				{
					pos.z -=  2 * walkSpeed;
					if (abs(player.x - pos.x) < abs(player.z - pos.z)) {
						viewDir = DOWN;

					}
				}
			}
		}
		else
		{
			if (moveCount == 0)
			{
				viewDir = (enum Direction)(rand() % 4);
				moveCount = 30;
			}

			switch (viewDir)
			{
			case 0: // Left Up Right Down
				if (canGo)
				{
					pos.x -= walkSpeed;
				}
				break;
			case 1:
				if (canGo)
				{
					pos.z += walkSpeed;
				}
				break;
			case 2:
				if (canGo)
				{
					pos.x += walkSpeed;
				}
				break;
			case 3:
				if (canGo)
				{
					pos.z -= walkSpeed;
				}
				break;
			}
			moveCount--;
		}
	}
}

void Enemy::goBack()
{
	switch (viewDir)
	{
	case 0: // Left Up Right Down

			pos.x += walkSpeed;
		break;
	case 1:

			pos.z -= walkSpeed;

		break;
	case 2:

			pos.x -= walkSpeed;
		
		break;
	case 3:

			pos.z += walkSpeed;
		
		break;
	}
}

void Enemy::display(Mesh* mesh, Camera& camera, int frame)
{

	glm::mat4 Projection = camera.toProjMatrix();
	glm::mat4 View = camera.toViewMatrix();
	float scaleFactor = mesh->getScaleFactor();
	glm::mat4 World = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, pos.z));


	glm::mat4 Model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))*glm::scale(glm::mat4(1.0f),glm::vec3(scaleFactor, scaleFactor, scaleFactor));




	glm::mat4 mw = World * Model;

	glUniformMatrix4fv(projID, 1, GL_FALSE, &Projection[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &mw[0][0]);
	mesh->render(frame);
}



glm::vec3 Enemy::getPos()

{
	return this->pos;
}



void Enemy::setPos(const glm::vec3& pos)

{
	this->pos = pos;
}



glm::vec2 Enemy::getDir()
{
	return this->dir;
}

void Enemy::setDir(const glm::vec2& dir)
{
	this->dir = dir;
}

void Enemy::moveRandom()
{
	glm::vec3 tmp = pos;

	tmp.x += rand() % 3 - 1;
	tmp.z += rand() % 3 - 1;
	setPos(tmp);
}