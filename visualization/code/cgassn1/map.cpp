#include "map.h"
#include "enemy.h"
#include "player.h"
#include "gun.h"
#include "wall.h"
#include "sys.h"
#include "util.h"
#include "texture.h"
#include "timer.h"


using namespace std;


Mesh * M_player;
Mesh * M_gun;
Mesh * M_enemy;
Mesh * M_wall;
Timer timer;


Map::Map()
{



	setShader();


	timer.init();

	M_enemy->init("cgassn1/resources/dummy_obj.obj");
	M_player->init("cgassn1/resources/dummy_obj.obj", false,true);
	M_gun->init("cgassn1/resources/M1911.obj", true);
	M_wall->init("cgassn1/resources/cube.obj", true);

	player = new Player();
	gun = new Gun();

	//listEnemy.push_back(new Enemy());
	//listWall.push_back(new Wall());
	gridLength = 50;//(M_wall->getScaleFactor());
	gridSize = 32;
	//printf("%f\n", gridLength);
}

void
Map::init(void)
{
	camera.init(glm::vec3(25, 50, 15), glm::vec2(0.0f, 180.0f));
	//enemy.init(glm::vec3(0, 0, 50), glm::vec2(0.0f, 0.0f));

	player->init(glm::vec3(25, 5, 15), glm::vec2(0.0f, 180.0f));
	gun->init(glm::vec3(0, 0, 80), glm::vec2(0.0f, 0.0f));
	// need to be iterative
	//wall.init(glm::vec3(-30, 0, 0), glm::vec2(0.0f, 0.0f));
	//listEnemy.front()->init(glm::vec3(0, 0, 50), glm::vec2(0.0f, 0.0f));
	//listWall.front()->init(glm::vec3(25, 0, 25), glm::vec2(0.0f, 0.0f));




	for (int x = 0; x < 1; x++)
		for (int y = 0; y < 1; y++)
		{
			listWall.push_back(new Wall(x * gridLength + 25, y * gridLength + 25));
		}



	



	remainingTime = CLOCKS_PER_SEC * 500;
	startTime = clock();
}

void
Map::display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_DEPTH_TEST);


	//glm::vec3 lightPos = glm::vec3(player->getPos().x, player->getPos().y + 50.f, player->getPos().z);//glm::vec3(300.0f, -100.0f , 300.0f);
	glm::vec3 lightPos = glm::vec3(300.0f, 50.0f , 300.0f);
	float yrad = glm::radians(player->getDir().y);
	glm::vec3 lightPos2 = lightPos;

	/*
	shaderUtil.bind();
	shaderUtil.Use();
	glm::mat4 Projection = camera.toProjMatrix();
	glm::mat4 View = camera.toViewMatrix();
	glm::mat4 World = glm::mat4(1.0f);
	glm::mat4 Model = glm::mat4(1.0f);
	glm::mat4 mw = World * Model;

	glUniformMatrix4fv(projID, 1, GL_FALSE, &Projection[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &mw[0][0]);
	glUniformMatrix4fv(ani, 1, GL_FALSE, &glm::mat4(1.0f)[0][0]);
	glUniform4fv(viewPosID, 1, &camera.getPos()[0]);
	glUniform3fv(light_ID, 1, &camera.getPos()[0]);
	glUniform3fv(light_ID2, 1, &lightPos2[0]);



	*/
	//if (!(player->status == KILLED || gameClear))
	//{
	//	//wall.display(M_wall, camera);
	//	//enemy.display(M_enemy, camera);
	//	player->display(M_player, camera, frame);
	//	gun->display(M_gun, camera, frame);

	//	for (std::list<Bullet*>::iterator it = listBullet.begin(); it != listBullet.end(); it++)
	//		(*it)->display(M_wall, camera, frame);

	//	for (std::list<Enemy*>::iterator it = listEnemy.begin(); it != listEnemy.end(); it++)
	//		(*it)->display(M_enemy, camera, frame);
	//	//for (std::list<Wall*>::iterator it = listWall.begin(); it != listWall.end(); it++)
	//	//	(*it)->display(M_wall, camera, frame);


	//}
	//else
	//{
	//	player->display(M_player, camera, frame);
	//	gun->display(M_gun, camera, frame);
	//}
	


	frame = (frame + 1) % 2;
	shaderWallUtil.bind();
	shaderWallUtil.Use();
	if (!(player->status == KILLED || gameClear))
	{
		glUniform3fv(LightID, 1, &lightPos[0]);
		glUniform3fv(LightID2, 1, &lightPos2[0]);
		//wall.display(M_wall, camera);
		//enemy.display(M_enemy, camera);


		for (std::list<Wall*>::iterator it = listWall.begin(); it != listWall.end(); it++)
			 (*it)->display(M_wall, camera, frame);


	}
	//shaderWallUtil.unbind();

	//timer.bind();
	//timer.display(camera, frame);

	//timer.unbind();

}

void
Map::reshape(int w, int h)
{

}
void
Map::moveObjects()
{

	//checkWall();
	moveEnemy();
	checkWallBullet();
	checkWallEnemy();
	checkEnemyKill();
	checkPlayerKill();
	player->update();
	

	glm::vec3 tmp = player->getPos();
	
	gun->setPos(tmp);
	timer.setPos(tmp);

	glm::vec2 rot = player->getDir();
	float yrad = glm::radians(rot.y);
	
	gun->setDir(rot);
	gun->update();
	if (camera.mode == FPS)
	{
		tmp.y += 45.0f;
		camera.setPos(glm::vec3(tmp.x + 5.f * glm::sin(yrad),tmp.y,tmp.z- 5.0f*glm::cos(yrad)));
		camera.setRot(rot);
	}
	else if (camera.mode == TPS)
	{
		tmp.y += 120.f;
		tmp.x -= 80.f * glm::sin(yrad);
		tmp.z += 80.f * glm::cos(yrad);
		camera.setPos(tmp);
		rot.x += 30.f;
		camera.setRot(rot);
	}



	camera.update(1.0);
	
	// check bullet routine
	for (std::list<Bullet*>::iterator it = listBullet.begin(); it != listBullet.end(); it++)
	{
		if ((*it)->status == ALIVE)
		{
			(*it)->update();
			//printf("%f\n", glm::distance((*it)->getPos(), listEnemy.front()->getPos()));
		}
	}

	std::list<Bullet*>::iterator it = listBullet.begin();

	while (it != listBullet.end())
	{

		if ((*it)->status == KILLED)
		{
			Bullet * tmp = NULL;
			tmp = *it;
			it = listBullet.erase(it);

			delete tmp;
		}
		else it++;
	}




	if (player->status == KILLED || gameClear)
	{

			if (keyboardBuffer['q'])
			{

				glutDestroyWindow(windowId);
				exit(0);
			}

			if (keyboardBuffer['r'])
			{

				gameReset();
			
			}

	}

	if (keyboardBuffer['t'])
	{
		if (is_phong) {
			is_phong = false;
			setShader();
		}
		else {
			is_phong = true;
			setShader();
		}
	}
}

void Map::checkWall()
{
	std::list<Wall*>::iterator it = listWall.begin();

	while (it != listWall.end())
	{
		if (glm::distance(player->getPos(), (*it)->getPos()) < 34.f)
		{
			player->goBack();
			player->canGo = false;
			return;
		}
		it++;
	}

	player->canGo = true;
}

void Map::checkWallBullet()
{
	for (std::list<Bullet*>::iterator it = listBullet.begin(); it != listBullet.end(); it++)
	{
		int fx, fy, bx, by;
		glm::vec3 tmp = (*it)->getPos();
		fx = pos2idx(tmp.x);
		fy = pos2idx(tmp.z);
		bx = fx;
		by = fy;

		/*float colDistance = 17.0f;
		float dist = 0;
		switch ((*it)->viewDir)
		{
		case UP:
			fy -= 1;
			by += 1;
			break;
		case DOWN:
			fy += 1;
			by -= 1;
			break;
		case LEFT:
			fx -= 1;
			bx += 1;
			break;
		case RIGHT:
			fx += 1;
			bx -= 1;
			break;
		}

		glm::vec3 tmpDist;
		if (fx == -1 || fx == 32 || fy == -1 || fy == 32 || objMap[fx][fy] == WALL)
		{
			switch ((*it)->viewDir)
			{
			case UP:
			case DOWN:
				tmpDist = glm::vec3(tmp.x, 0, idx2pos(fy));
				break;

			case LEFT:
			case RIGHT:
				tmpDist = glm::vec3(idx2pos(fx), 0, tmp.z);
				break;
			}
		}*/

		//if (colDistance >= glm::distance(tmpDist, tmp))
		if (fx == -1 || fx == 32 || fy == -1 || fy == 32 || objMap[fx][fy] == WALL)
		{
			(*it)->status = KILLED;
		}

		
	}
}

void Map::checkEnemyKill()
{
	float killDistance = 25.f;
	if (listBullet.empty())
	{
		return;
	}
	//puts ("test");
	for (std::list<Bullet*>::iterator it = listBullet.begin(); it != listBullet.end(); it++)
	{

		for (std::list<Enemy*>::iterator itEnemy = listEnemy.begin(); itEnemy != listEnemy.end(); itEnemy++)
		{
			
			

			if (killDistance > glm::distance ((*it)->getPos(), (*itEnemy)->getPos()))
			{
				(*itEnemy)->status = KILLED;
				(*it)->status = KILLED;
			}
		}
	}

	std::list<Enemy*>::iterator itEnemy = listEnemy.begin();

	while (itEnemy != listEnemy.end())
	{
		if ((*itEnemy)->status == KILLED)
		{
			Enemy *tmp = NULL;
			tmp = *itEnemy;

			itEnemy = listEnemy.erase(itEnemy);

			delete tmp;
		}
		else itEnemy++;
	}

	if (listEnemy.empty())
	{
		gameClear = true;
	}
}

void Map::checkPlayerKill()
{
	std::list<Enemy*>::iterator it = listEnemy.begin();

	while (it != listEnemy.end())
	{
		if (glm::distance(player->getPos(), (*it)->getPos()) < 10.f)
		{
			player->status = KILLED;
		}
		it++;
	}
}

void Map::checkWallEnemy()
{
	for (std::list<Enemy*>::iterator it = listEnemy.begin(); it != listEnemy.end(); it++)
	{
		int fx, fy, bx, by;
		glm::vec3 tmp = (*it)->getPos();
		fx = pos2idx(tmp.x);
		fy = pos2idx(tmp.z);
		bx = fx;
		by = fy;

		float colDistance = 34.0f;
		float dist = 0;
		switch ((*it)->viewDir)
		{
		case UP:
			fy -= 1;
			by += 1;
			break;
		case DOWN:
			fy += 1;
			by -= 1;
			break;
		case LEFT:
			fx -= 1;
			bx += 1;
			break;
		case RIGHT:
			fx += 1;
			bx -= 1;
			break;
		}

		glm::vec3 tmpDist;
		if (fx == -1 || fx == 32 || fy == -1 || fy == 32 || objMap[fx][fy] == WALL)
		{
			switch ((*it)->viewDir)
			{
			case UP:
			case DOWN:
				tmpDist = glm::vec3(tmp.x, 0, idx2pos(fy));
				break;

			case LEFT:
			case RIGHT:
				tmpDist = glm::vec3(idx2pos(fx), 0, tmp.z);
				break;
			}
		}

		if (colDistance >= glm::distance(tmpDist, tmp))
		{
			(*it)->canGo = false;
			(*it)->goBack();
		}

		else (*it)->canGo = true;
	}

}


void Map::moveEnemy()
{
	for (std::list<Enemy*>::iterator it = listEnemy.begin(); it != listEnemy.end(); it++)
		(*it)->update(player->getPos());
}

void Map::gameReset()
{
	std::list<Enemy*>::iterator itEnemy = listEnemy.begin();

	while (itEnemy != listEnemy.end())
	{
			Enemy *tmp = NULL;
			tmp = *itEnemy;
			itEnemy = listEnemy.erase(itEnemy);
			delete tmp;
	}

	std::list<Bullet*>::iterator itBullet = listBullet.begin();

	while (itBullet != listBullet.end())
	{
		Bullet *tmp = NULL;
		tmp = *itBullet;
		itBullet = listBullet.erase(itBullet);
		delete tmp;
	}

	std::list<Wall*>::iterator itWall = listWall.begin();

	while (itWall != listWall.end())
	{
		Wall *tmp = NULL;
		tmp = *itWall;
		itWall = listWall.erase(itWall);
		delete tmp;
	}

	init();
}




void Map::checkTimeout()
{
	if (clock() > startTime + remainingTime)
		player->status = KILLED;
}

void Map::displayTime(void) {
	glColor3f(0.f, 0.f, 0.f);
	char buf[100] = { 0 };
	std::string str = "Remain Time : " + std::to_string((int)((remainingTime - clock()) / 1000));
	std::cout << remainingTime - clock() << std::endl;
	sprintf_s(buf, str.c_str());
	renderbitmap(50, 0,50, GLUT_BITMAP_TIMES_ROMAN_24, buf);
}



void Map::setShader() {

	glClearColor(1.0, 1.0, 1.0, 0.0);

	// wall shader
	if (is_phong) {
		shaderWallUtil.Load("cgassn1/shaders/vsWall_SH_refined.glsl", "cgassn1/shaders/fsWall_SH_refined.glsl");
	}
	else {
		shaderWallUtil.Load("cgassn1/shaders/vsWall_SH.glsl", "cgassn1/shaders/fsWall_SH.glsl");
	}
	shaderWallUtil.bind();
	// Get a handle for our "MVP" uniform
	MatrixID = glGetUniformLocation(shaderWallUtil.getProgram(), "MVP");
	ViewMatrixID = glGetUniformLocation(shaderWallUtil.getProgram(), "V");
	ModelMatrixID = glGetUniformLocation(shaderWallUtil.getProgram(), "M");
	ModelView3x3MatrixID = glGetUniformLocation(shaderWallUtil.getProgram(), "MV3x3");

	// Load the texture
	DiffuseTexture = loadBMP_custom("jake_face.bmp");
	NormalTexture = loadBMP_custom("jake_face_normals.bmp");
	//DiffuseTexture = loadDDS("diffuse.DDS");
	//NormalTexture = loadBMP_custom("normal.bmp");
	SpecularTexture = loadDDS("specular.DDS");

	// Get a handle for our "myTextureSampler" uniform
	DiffuseTextureID = glGetUniformLocation(shaderWallUtil.getProgram(), "DiffuseTextureSampler");
	NormalTextureID = glGetUniformLocation(shaderWallUtil.getProgram(), "NormalTextureSampler");
	SpecularTextureID = glGetUniformLocation(shaderWallUtil.getProgram(), "SpecularTextureSampler");

	// Read our .obj file

	bool res = loadOBJ("cgassn1/resources/jake_face.obj", vertices, uvs, normals);

	computeTangentBasis(
		vertices, uvs, normals, // input
		tangents, bitangents    // output
	);


	indexVBO_TBN(
		vertices, uvs, normals, tangents, bitangents,
		indices, indexed_vertices, indexed_uvs, indexed_normals, indexed_tangents, indexed_bitangents
	);

	// Load it into a VBO


	glGenBuffers(1, &vertexbufferWall);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferWall);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvbufferWall);
	glBindBuffer(GL_ARRAY_BUFFER, uvbufferWall);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normalbufferWall);
	glBindBuffer(GL_ARRAY_BUFFER, normalbufferWall);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

	glGenBuffers(1, &tangentbufferWall);
	glBindBuffer(GL_ARRAY_BUFFER, tangentbufferWall);
	glBufferData(GL_ARRAY_BUFFER, indexed_tangents.size() * sizeof(glm::vec3), &indexed_tangents[0], GL_STATIC_DRAW);

	glGenBuffers(1, &bitangentbufferWall);
	glBindBuffer(GL_ARRAY_BUFFER, bitangentbufferWall);
	glBufferData(GL_ARRAY_BUFFER, indexed_bitangents.size() * sizeof(glm::vec3), &indexed_bitangents[0], GL_STATIC_DRAW);

	// Generate a buffer for the indices as well
	glGenBuffers(1, &elementbufferWall);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbufferWall);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);


	LightID = glGetUniformLocation(shaderWallUtil.getProgram(), "LightPosition_worldspace");
	LightID2 = glGetUniformLocation(shaderWallUtil.getProgram(), "LightPosition_worldspace2");


	// Bind our diffuse texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, DiffuseTexture);
	// Set our "DiffuseTextureSampler" sampler to use Texture Unit 0
	glUniform1i(DiffuseTextureID, 0);

	// Bind our normal texture in Texture Unit 1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, NormalTexture);
	// Set our "NormalTextureSampler" sampler to use Texture Unit 1
	glUniform1i(NormalTextureID, 1);

	// Bind our specular texture in Texture Unit 2
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, SpecularTexture);
	// Set our "SpecularTextureSampler" sampler to use Texture Unit 2
	glUniform1i(SpecularTextureID, 2);


	//test
	// Bind our diffuse texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, DiffuseTexture);
	// Set our "DiffuseTextureSampler" sampler to use Texture Unit 0
	glUniform1i(DiffuseTextureID, 0);

	// Bind our normal texture in Texture Unit 1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, NormalTexture);
	// Set our "NormalTextureSampler" sampler to use Texture Unit 1
	glUniform1i(NormalTextureID, 1);

	// Bind our specular texture in Texture Unit 2
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, SpecularTexture);
	// Set our "SpecularTextureSampler" sampler to use Texture Unit 2
	glUniform1i(SpecularTextureID, 2);


	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferWall);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbufferWall);
	glVertexAttribPointer(
		1,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// 3rd attribute buffer : normals
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalbufferWall);
	glVertexAttribPointer(
		2,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// 4th attribute buffer : tangents
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, tangentbufferWall);
	glVertexAttribPointer(
		3,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// 5th attribute buffer : bitangents
	glEnableVertexAttribArray(4);
	glBindBuffer(GL_ARRAY_BUFFER, bitangentbufferWall);
	glVertexAttribPointer(
		4,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbufferWall);
	shaderWallUtil.unbind();
}