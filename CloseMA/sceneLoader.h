#ifndef SCENELOADER_H
#define SCENELOADER_H
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
//#include <SDL/SDL_image.h>
#include "mesho.h"

class meshLoader{
	std::vector<mesh*> meshes;
	void recursiveProcess(aiNode* node,const aiScene* scene);
	void processMesh(aiMesh* mesh,const aiScene* scene);
	unsigned int loadTexture(const char* filename);
	public:
		meshLoader(const char* filename);
		~meshLoader();
		void draw(unsigned int programId);
		std::vector<mesh*>& getMeshes();
};

#endif
