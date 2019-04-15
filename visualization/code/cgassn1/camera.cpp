#include "sys.h"
#include "util.h"




void Camera::init(const glm::vec3& pos, const glm::vec2& rot)

{

	this->setPos(pos);

	this->setRot(rot);

	this->rotSpeed = .5f;

	this->mode = FPS;

	modeDelay = CLOCKS_PER_SEC / 2;
	lastConvert = 0;
}



void Camera::update(float delta)

{
	int i = 0;

	float yrad = glm::radians(rot.y);

	float sp = this->walkSpeed;

	if (lastConvert + modeDelay < clock())
	{
		lastConvert = 0;
	}

	for (i = 0; i < KEYBOARD_BUFFER_SIZE; i++)
	{
		if (keyboardBuffer[i])
		{
	/*		if (i == 'a')
			{
				rot.y -= this->rotSpeed;
			}

			if (i == 's')
			{
				pos.x -= sp * glm::sin(yrad) * delta;
				pos.z += sp * glm::cos(yrad) * delta;
			}


			if (i == 'd')
			{
				rot.y += this->rotSpeed;
			}


			if (i == 'w')
			{
				pos.x += sp * glm::sin(yrad) * delta;
				pos.z -= sp * glm::cos(yrad) * delta;
			}

			if (i == 'r')
			{
				rot.x += this->rotSpeed;
			}
			if (i == 'f')
			{
				rot.x -= this->rotSpeed;
			}
			if (i == 't')
			{
				pos.y += sp * glm::sin(yrad) * delta;
				pos.z -= sp * glm::cos(yrad) * delta;
			}
			if (i == 'g')
			{
				pos.y -= sp * glm::sin(yrad) * delta;
				pos.z += sp * glm::cos(yrad) * delta;
			}*/

			if (i == 'v' && lastConvert == 0)
			{
				convertMode();
				lastConvert = clock();
			}

		}
	}

}



glm::mat4 Camera::toViewMatrix()

{

	return

		glm::rotate(glm::mat4(1.0f), glm::radians(rot.x), glm::vec3(1.0f, 0.0f, 0.0f)) *

		glm::rotate(glm::mat4(1.0f), glm::radians(rot.y), glm::vec3(0.0f, 1.0f, 0.0f)) *

		glm::translate(glm::mat4(1.0f), -this->pos);
	 
}



glm::mat4 Camera::toProjMatrix()

{

	return glm::perspective(

		glm::radians(this->fov),

		(float)800 / (float)800,

		1.0f,

		3000.0f

	);

}



glm::vec3 Camera::getPos()

{

	return this->pos;

}



void Camera::setPos(const glm::vec3& pos)
{
	this->pos = pos;
}

glm::vec2 Camera::getRot()
{
	return this->rot;
}

void Camera::setRot(const glm::vec2& rot)
{
	this->rot = rot;
}



float Camera::getFOV()
{
	return this->fov;
}

void Camera::setFOV(float fov)
{
	this->fov = fov;
}

float Camera::getRotSpeed()
{
	return this->rotSpeed;
}

void Camera::setRotSpeed(float rotSpeed)
{
	this->rotSpeed = rotSpeed;
}

void Camera::convertMode()
{
	this->mode = this->mode == FPS ? TPS : FPS;
}