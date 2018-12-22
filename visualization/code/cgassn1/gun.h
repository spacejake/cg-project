#pragma once
#include "sys.h"

class Gun
{
private:
	glm::vec3 pos;
	glm::vec2 dir;

	float walkSpeed = 1.0f;
	clock_t bangDelay;
	clock_t lastbang;
public:

	void init(const glm::vec3& pos, const glm::vec2& rot);

	void update();
	void display(Mesh*, Camera&, int);

	glm::vec3 getPos();

	void setPos(const glm::vec3& pos);

	glm::vec2 getDir();

	void setDir(const glm::vec2& dir);
	void bang();
	bool canShoot();
};