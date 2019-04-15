#pragma once
#pragma once
#include "sys.h"

class Bullet
{
private:
	glm::vec3 pos;
	glm::vec2 dir;
	

	float walkSpeed = 10.0f;

	clock_t creationTime;
	clock_t lifeTime;

public:
	enum Status status;
	enum Direction viewDir;
	Bullet(const glm::vec3& pos, const glm::vec2& rot);
	Bullet(const glm::vec3& pos, const glm::vec2& rot, enum Direction);
	void init(const glm::vec3& pos, const glm::vec2& rot);

	void update();
	void display(Mesh*, Camera&, int);

	glm::vec3 getPos();

	void setPos(const glm::vec3& pos);

	glm::vec2 getDir();

	void setDir(const glm::vec2& dir);


};