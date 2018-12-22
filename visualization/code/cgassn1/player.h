#pragma once
#include "sys.h"

class Player
{
private:
	glm::vec3 pos;
	glm::vec2 dir;
	
	float walkSpeed = 5.0f;

	clock_t rotDelay;
	clock_t lastRot;


public:
	std::string name;
	enum Status status;
	enum Direction viewDir;
	bool canGo;
	void init(const glm::vec3& pos, const glm::vec2& rot);

	void update();
	void goBack();
	void display(Mesh*, Camera&, int);

	glm::vec3 getPos();

	void setPos(const glm::vec3& pos);

	glm::vec2 getDir();

	void setDir(const glm::vec2& dir);

};