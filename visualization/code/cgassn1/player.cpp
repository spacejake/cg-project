#include "player.h"
#include "util.h"



void Player::init(const glm::vec3& pos, const glm::vec2& dir)
{
	this->setPos(pos);
	this->setDir(dir);
	rotDelay = CLOCKS_PER_SEC / 2;
	lastRot = 0;


	this->viewDir = UP;
	canGo = true;
	status = ALIVE;
}

void Player::goBack()
{
	float yrad = glm::radians(dir.y);
	float delta = 1.0;
	float sp = this->walkSpeed;
	pos.x -= sp * glm::sin(yrad) * delta;
	pos.z += sp * glm::cos(yrad) * delta;
}

void Player::update(void)
{
	int i = 0;

	float yrad = glm::radians(dir.y);
	float delta = .3;
	float sp = this->walkSpeed;

	if (lastRot + rotDelay < clock())
	{
		lastRot = 0;
	}


	for (i = 0; i < KEYBOARD_BUFFER_SIZE; i++)
	{
		if (keyboardBuffer[i])
		{


			if (i == 's')
			{
				pos.x -= sp * glm::sin(yrad) * delta;
				pos.z += sp * glm::cos(yrad) * delta;
			}

			if (i == 'a')
			{
				dir.y -= .5f;
			}

			if (i == 'd')
			{
				dir.y += .5f;
			}

			if (i == 'q')
			{
				pos.y += 0.1*sp;
			}

			if (i == 'z')
			{
				pos.y -= 0.1*sp;
			}

			/*if (i == 'a' && lastRot == 0)

			{
				dir.y -= 90;
				this->viewDir = (enum Direction) ((this->viewDir + 3) % 4);
				lastRot = clock();
			}

			if (i == 'd' && lastRot == 0)
			{
				dir.y += 90;
				this->viewDir = (enum Direction) ((this->viewDir + 1) % 4);
				lastRot = clock();
			}*/

			
			if (i == 'w' && canGo)
			{
				pos.x += sp * glm::sin(yrad) * delta;
				pos.z -= sp * glm::cos(yrad) * delta;
			}

			//if (i == 'r')
			//{
			//	dir.x += 1.5f;
			//}
			//if (i == 'f')
			//{
			//	dir.x -= 1.5;
			//}
	/*		if (i == 't')
			{
				pos.y += sp * glm::sin(yrad) * delta;
				pos.z -= sp * glm::cos(yrad) * delta;
			}
			if (i == 'g')
			{
				pos.y -= sp * glm::sin(yrad) * delta;
				pos.z += sp * glm::cos(yrad) * delta;
			}*/



		}
	}

}


void Player::display(Mesh* mesh, Camera& camera, int frame)
{
	glm::mat4 Projection = camera.toProjMatrix();
	glm::mat4 View = camera.toViewMatrix();
	float scaleFactor = mesh->getScaleFactor();
	glm::mat4 World = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, pos.z));
	glm::mat4 Model =	glm::rotate(glm::mat4(1.0f), glm::radians(-camera.getRot().y + 180.f), glm::vec3(0.0f, 1.0f, 0.0f))
						* glm::scale(glm::mat4(1.0f), glm::vec3(scaleFactor, scaleFactor, scaleFactor));

	glm::mat4 mw = World * Model;

	glUniformMatrix4fv(projID, 1, GL_FALSE, &Projection[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &mw[0][0]);

	mesh->render(frame);
}


glm::vec3 Player::getPos()

{

	return this->pos;

}



void Player::setPos(const glm::vec3& pos)

{

	this->pos = pos;

}



glm::vec2 Player::getDir()

{

	return this->dir;

}



void Player::setDir(const glm::vec2& dir)
{
	this->dir = dir;
}
