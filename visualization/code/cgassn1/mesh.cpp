#include "sys.h"
#include "util.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <IL\il.h>

#define abs(x) ((x)<0 ? -(x):(x))

Mesh::Mesh() {
	memcpy(current_matrix, glm::value_ptr(glm::mat4(1.0f)), sizeof(current_matrix));
}

void Mesh::init(std::string path, bool isStatic_, bool isPlayer_)
{
	isStatic = isStatic_;
	isPlayer = isPlayer_;
	Assimp::Importer importer;
	const aiScene* sc = importer.ReadFile(path.c_str(),
			aiProcess_Triangulate 
		|	aiProcess_FlipUVs 
		|	aiProcess_GenSmoothNormals 
		|	aiProcess_CalcTangentSpace 
		//|	aiProcess_JoinIdenticalVertices
	);

	if (!sc || sc->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !sc->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	aiVector3D scene_min, scene_max, scene_center;
	get_bounding_box(sc, &scene_min, &scene_max);
	float tmp;
	tmp = scene_max.x - scene_min.x;
	tmp = scene_max.y - scene_min.y > tmp ? scene_max.y - scene_min.y : tmp;
	tmp = scene_max.z - scene_min.z > tmp ? scene_max.z - scene_min.z : tmp;
	this->scaleFactor = 50.f / tmp;

	recursiveNodeProcess(sc->mRootNode);

	LoadGLTextures(sc);

	struct MyMesh aMesh;
	struct MyMaterial aMat;
	GLuint buffer;

	// For each mesh
	for (unsigned int n = 0; n < sc->mNumMeshes; ++n)
	{
		const aiMesh* mesh = sc->mMeshes[n];

		// create array with faces
		// have to convert from Assimp format to array
		unsigned int *faceArray;
		faceArray = (unsigned int *)malloc(sizeof(unsigned int) * mesh->mNumFaces * 3);
		unsigned int faceIndex = 0;

		for (unsigned int t = 0; t < mesh->mNumFaces; ++t) {
			const aiFace* face = &mesh->mFaces[t];

			memcpy(&faceArray[faceIndex], face->mIndices, 3 * sizeof(unsigned int));
			faceIndex += 3;
		}
		aMesh.numFaces = sc->mMeshes[n]->mNumFaces;

		// generate Vertex Array for mesh
		glGenVertexArrays(1, &(aMesh.vao));
		glBindVertexArray(aMesh.vao);

		// buffer for faces
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh->mNumFaces * 3, faceArray, GL_STATIC_DRAW);

		// buffer for vertex positions
		if (mesh->HasPositions()) {
			glGenBuffers(1, &buffer);
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh->mNumVertices, mesh->mVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
		}

		// buffer for vertex normals
		if (mesh->HasNormals()) {
			glGenBuffers(1, &buffer);
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh->mNumVertices, mesh->mNormals, GL_STATIC_DRAW);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, 0, 0, 0);
		}

		// buffer for vertex texture coordinates
		if (mesh->HasTextureCoords(0)) {
			float *texCoords = (float *)malloc(sizeof(float) * 2 * mesh->mNumVertices);
			for (unsigned int k = 0; k < mesh->mNumVertices; ++k) {

				texCoords[k * 2] = mesh->mTextureCoords[0][k].x;
				texCoords[k * 2 + 1] = mesh->mTextureCoords[0][k].y;

			}
			glGenBuffers(1, &buffer);
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * mesh->mNumVertices, texCoords, GL_STATIC_DRAW);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, 0, 0, 0);
		}

		// unbind buffers
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// create material uniform buffer
		aiMaterial *mtl = sc->mMaterials[mesh->mMaterialIndex];

		aiString texPath;	//contains filename of texture
		if (AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, 0, &texPath)) {
			//bind texture
			unsigned int texId = textureIdMap[texPath.data];
			aMesh.texIndex = texId;
			aMat.texCount = 1;
		}
		else
			aMat.texCount = 0;

		float c[4];
		set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
		aiColor4D diffuse;
		if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
			color4_to_float4(&diffuse, c);
		memcpy(aMat.diffuse, c, sizeof(c));

		set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
		aiColor4D ambient;
		if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
			color4_to_float4(&ambient, c);
		memcpy(aMat.ambient, c, sizeof(c));

		set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
		aiColor4D specular;
		if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
			color4_to_float4(&specular, c);
		memcpy(aMat.specular, c, sizeof(c));

		set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
		aiColor4D emission;
		if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
			color4_to_float4(&emission, c);
		memcpy(aMat.emissive, c, sizeof(c));

		float shininess = 0.0;
		unsigned int max;
		aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
		aMat.shininess = shininess;

		glGenBuffers(1, &(aMesh.uniformBlockIndex));
		glBindBuffer(GL_UNIFORM_BUFFER, aMesh.uniformBlockIndex);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(aMat), (void *)(&aMat), GL_STATIC_DRAW);

		myMeshes.push_back(aMesh);
	}

}

void Mesh::render(int frame)
{
	glUniform1i(texUnit, 2);
	if(frame ==0)
		sprite = (sprite + 1) % fps;
	for (unsigned int i = 0; i < ai_nodes.size(); i++) {
		// draw all meshes assigned to this node
		for (unsigned int n = 0; n < ai_nodes[i]->mNumMeshes; ++n) {
			if (!isStatic) {
				glUniformMatrix4fv(ani, 1, GL_FALSE, &saved_matrices[sprite].at(i)[0][0]);
			}
			else {
				glUniformMatrix4fv(ani, 1, GL_FALSE, &saved_matrices[0].at(i)[0][0]);
			}

			// bind material uniform
			
			glBindBufferRange(GL_UNIFORM_BUFFER, materialUniLoc, myMeshes[ai_nodes[i]->mMeshes[n]].uniformBlockIndex, 0, sizeof(struct MyMaterial));
			// bind texture
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, myMeshes[ai_nodes[i]->mMeshes[n]].texIndex); 
			// bind VAO
			glBindVertexArray(myMeshes[ai_nodes[i]->mMeshes[n]].vao);

			// draw
			glDrawElements(GL_TRIANGLES, myMeshes[ai_nodes[i]->mMeshes[n]].numFaces * 3, GL_UNSIGNED_INT, 0);
		}
		
	}
}


void Mesh::recursiveNodeProcess(aiNode * node)
{
	aiNode * nnode = (aiNode*)malloc(sizeof(aiNode));
	memcpy(nnode, node, sizeof(aiNode));
	nnode->mMeshes = (unsigned*)malloc(sizeof(unsigned)*node->mNumMeshes);
	memcpy(nnode->mMeshes, node->mMeshes, sizeof(unsigned)*node->mNumMeshes);
	

	pushMatrix();

	if (!isStatic) {
		std::string a = node->mName.C_Str();
		std::vector<std::string> tokens = split_string(a);
		std::string parts = tokens.at(tokens.size() - 1);

		if (std::find(tokens.begin(), tokens.end(), "dummy_rshoulder") != tokens.end()) {
			if (isPlayer) {
				glm::mat4 temp = glm::translate(glm::mat4(1.0f), glm::vec3(-15, 140, 0))*glm::rotate(glm::mat4(1.0), (3.14f / 2 + 3.14f / 24), glm::vec3(0, 1, 0))* glm::translate(glm::mat4(1.0f), glm::vec3(15, -140, 0));
				memcpy(current_matrix, glm::value_ptr(temp), sizeof(current_matrix));
				for (unsigned int i = 0; i < fps; i++) {
					saveMatrix(i);
				}
			}
			else {
				for (unsigned int i = 0; i < fps; i++) {
					int dist = fps/2-abs((int)i-15);
					glm::mat4 temp = glm::translate(glm::mat4(1.0f), glm::vec3(-15, 140, 0))*glm::rotate(glm::mat4(1.0), (3.14f / 4)-(3.14f/2)*((float)dist/(fps/2)), glm::vec3(0, 1, 0))*glm::rotate(glm::mat4(1.0), (3.14f / 4), glm::vec3(0, 0, 1))* glm::translate(glm::mat4(1.0f), glm::vec3(15, -140, 0));
					memcpy(current_matrix, glm::value_ptr(temp), sizeof(current_matrix));
					saveMatrix(i);
				}
			}

		}
		else if (std::find(tokens.begin(), tokens.end(), "dummy_lshoulder") != tokens.end()) {
			for (unsigned int i = 0; i < fps; i++) {
				int dist = fps / 2 - abs((int)i - 15);
				glm::mat4 temp = glm::translate(glm::mat4(1.0f), glm::vec3(15, 140, 0))*glm::rotate(glm::mat4(1.0), (3.14f / 4) - (3.14f / 2)*((float)dist / (fps / 2)), glm::vec3(0, 1, 0))*glm::rotate(glm::mat4(1.0), -(3.14f / 4), glm::vec3(0, 0, 1))* glm::translate(glm::mat4(1.0f), glm::vec3(-15, -140, 0));
				memcpy(current_matrix, glm::value_ptr(temp), sizeof(current_matrix));
				saveMatrix(i);
			}
		}
		else if (std::find(tokens.begin(), tokens.end(), "dummy_rpelvis") != tokens.end()) {
			for (unsigned int i = 0; i < fps; i++) {
				int dist = fps / 2 - abs((int)i - 15);
				glm::mat4 temp = glm::translate(glm::mat4(1.0f), glm::vec3(-8, 90, 0))*glm::rotate(glm::mat4(1.0), (3.14f / 4) - (3.14f / 2) *((float)dist / (fps / 2)), glm::vec3(1, 0, 0))* glm::translate(glm::mat4(1.0f), glm::vec3(8, -90, 0));
				memcpy(current_matrix, glm::value_ptr(temp), sizeof(current_matrix));
				saveMatrix(i);
			}
		}
		else if (std::find(tokens.begin(), tokens.end(), "dummy_lpelvis") != tokens.end()) {
			for (unsigned int i = 0; i < fps; i++) {
				int dist = fps / 2 - abs((int)i - 15);
				glm::mat4 temp = glm::translate(glm::mat4(1.0f), glm::vec3(8, 90, 0))*glm::rotate(glm::mat4(1.0), -(3.14f / 4) + (3.14f / 2) *((float)dist / (fps / 2)), glm::vec3(1, 0, 0))* glm::translate(glm::mat4(1.0f), glm::vec3(-8, -90, 0));
				memcpy(current_matrix, glm::value_ptr(temp), sizeof(current_matrix));
				saveMatrix(i);
			}
		}
		else {
			for (unsigned int i = 0; i < fps; i++) {
				saveMatrix(i);
			}
		}
		
	}
	else {
		saveMatrix(0);
	}
	
	ai_nodes.push_back(nnode);

	for (int i = 0; i < node->mNumChildren; i++)
	{
		recursiveNodeProcess(node->mChildren[i]);
	}
	popMatrix();
}


#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

void Mesh::get_bounding_box_for_node(const aiScene* scene, const aiNode* nd,
	aiVector3D* min,
	aiVector3D* max)

{
	aiMatrix4x4 prev;
	unsigned int n = 0, t;

	for (; n < nd->mNumMeshes; ++n) {
		const aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
		for (t = 0; t < mesh->mNumVertices; ++t) {

			aiVector3D tmp = mesh->mVertices[t];

			min->x = aisgl_min(min->x, tmp.x);
			min->y = aisgl_min(min->y, tmp.y);
			min->z = aisgl_min(min->z, tmp.z);

			max->x = aisgl_max(max->x, tmp.x);
			max->y = aisgl_max(max->y, tmp.y);
			max->z = aisgl_max(max->z, tmp.z);
		}
	}

	for (n = 0; n < nd->mNumChildren; ++n) {
		get_bounding_box_for_node(scene, nd->mChildren[n], min, max);
	}
}


void Mesh::get_bounding_box(const aiScene* scene, aiVector3D* min, aiVector3D* max)
{

	min->x = min->y = min->z = 1e10f;
	max->x = max->y = max->z = -1e10f;
	get_bounding_box_for_node(scene, scene->mRootNode, min, max);
}

float Mesh::getScaleFactor(void) {
	return scaleFactor;
}

void Mesh::pushMatrix() {

	float *aux = (float *)malloc(sizeof(float) * 16);
	memcpy(aux, current_matrix, sizeof(float) * 16);
	matrixStack.push_back(aux);
}

void Mesh::popMatrix() {
	float *m = matrixStack[matrixStack.size() - 1];
	memcpy(current_matrix, m, sizeof(float) * 16);
	matrixStack.pop_back();
	free(m);
}

void Mesh::saveMatrix(int frame_) {
	glm::mat4 temp = glm::make_mat4(current_matrix);
	saved_matrices[frame_].push_back(temp);
}


int Mesh::LoadGLTextures(const aiScene* scene)
{
	ILboolean success;

	/* initialization of DevIL */
	ilInit();

	/* scan scene's materials for textures */
	for (unsigned int m = 0; m < scene->mNumMaterials; ++m)
	{
		int texIndex = 0;
		aiString path;	// filename

		aiReturn texFound = scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
		while (texFound == AI_SUCCESS) {
			//fill map with textures, OpenGL image ids set to 0
			textureIdMap[path.data] = 0;
			// more textures?
			texIndex++;
			texFound = scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
		}
	}

	int numTextures = textureIdMap.size();

	/* create and fill array with DevIL texture ids */
	ILuint* imageIds = new ILuint[numTextures];
	ilGenImages(numTextures, imageIds);

	/* create and fill array with GL texture ids */
	GLuint* textureIds = new GLuint[numTextures];
	glGenTextures(numTextures, textureIds); /* Texture name generation */

	/* get iterator */
	std::map<std::string, GLuint>::iterator itr = textureIdMap.begin();
	int i = 0;
	for (; itr != textureIdMap.end(); ++i, ++itr)
	{
		//save IL image ID
		std::string filename = (*itr).first;  // get filename
		(*itr).second = textureIds[i];	  // save texture id for filename in map

		ilBindImage(imageIds[i]); /* Binding of DevIL image name */
		ilEnable(IL_ORIGIN_SET);
		ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
		success = ilLoadImage((ILstring)filename.c_str());

		if (success) {
			/* Convert image to RGBA */
			ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

			/* Create and load textures to OpenGL */
			glBindTexture(GL_TEXTURE_2D, textureIds[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH),
				ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE,
				ilGetData());
		}
		else
			printf("Couldn't load Image: %s\n", filename.c_str());
	}
	/* Because we have already copied image data into texture data
	we can release memory used by image. */
	ilDeleteImages(numTextures, imageIds);

	//Cleanup
	delete[] imageIds;
	delete[] textureIds;

	//return success;
	return true;
}