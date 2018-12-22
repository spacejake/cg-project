#pragma once
#include "sys.h"

class Wall
{
private:
	glm::vec3 pos;
	glm::vec2 dir;


	float walkSpeed = 1.0f;

public:

	Wall();
	Wall(float, float);
	Wall(float, float, float);
	void init(const glm::vec3& pos, const glm::vec2& rot);

	void update();
	void display(Mesh*, Camera&, int);

	glm::vec3 getPos();

	void setPos(const glm::vec3& pos);

	glm::vec2 getDir();

	void setDir(const glm::vec2& dir);

};