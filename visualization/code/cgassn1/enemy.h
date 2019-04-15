#pragma once
#include "sys.h"

class Enemy
{
private:
	glm::vec3 pos;
	glm::vec2 dir;


	float walkSpeed = 1.0f;
	int moveCount = 30;

public:
	enum Status status;
	enum Direction viewDir;
	bool canGo;
	Enemy(float, float, enum Direction);
	void init(const glm::vec3& pos, const glm::vec2& rot);

	void update(glm::vec3);
	void display(Mesh*, Camera&, int);

	glm::vec3 getPos();

	void setPos(const glm::vec3& pos);

	glm::vec2 getDir();

	void setDir(const glm::vec2& dir);

	void moveRandom();
	void goBack();

};