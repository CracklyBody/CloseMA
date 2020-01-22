#pragma once
#ifndef ANIMATION_H
#define ANIMATION_H
#include <iostream>
#include <stb-master/stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/ai_assert.h>
#include <assimp/postprocess.h>
#include <vector>
#include <assimp/mesh.h>


class AnimationUtil
{
public:
	void loadAnimationFile(char* path)
	{
		Assimp::Importer  importer;
		const aiScene * scene = importer.ReadFile(path,
			aiProcess_Triangulate |
			aiProcess_GenSmoothNormals |
			aiProcess_FlipUVs |
			aiProcess_CalcTangentSpace |
			aiProcess_LimitBoneWeights
		);

		if (scene->mNumAnimations == 0)
		{
			std::cout << "File does not cpntain any animations\n";
			std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
			
		}

		aiMesh* mesh = scene->mMeshes[0];

		int sizeOfVertex = 19;
		int sizeOfVertexUnrigged = 11;


		float *array = new float[mesh->mNumVertices * sizeOfVertex];
		int index = 0;
		for (int v = 0; v < mesh->mNumVertices; v++)
		{

			aiVector3D position = aiVector3D(mesh->mVertices->x, mesh->mVertices->y, mesh->mVertices->z);
			aiVector3D normal = aiVector3D(mesh->mNormals->x, mesh->mNormals->y, mesh->mNormals->z);
			aiVector3D tangent = aiVector3D(mesh->mTangents->x, mesh->mTangents->y, mesh->mTangents->z);
			aiVector3D texCoord = aiVector3D(mesh->mTextureCoords[0]->x, mesh->mTextureCoords[0]->y, mesh->mTextureCoords[0]->z);

			array[index++] = position.x;
			array[index++] = position.y;
			array[index++] = position.z;

			array[index++] = normal.x;
			array[index++] = normal.y;
			array[index++] = normal.z;

			array[index++] = tangent.x;
			array[index++] = tangent.y;
			array[index++] = tangent.z;

			array[index++] = 0;
			array[index++] = 0;
			array[index++] = 0;

			array[index++] = 0;
			array[index++] = 0;
			array[index++] = 0;

		}

		int WEIGHTS_PER_VERTEX = 4;

		int boneArraysSize = mesh->mNumVertices * WEIGHTS_PER_VERTEX;

		std::vector<int> bonesIDs;
		bonesIDs.resize(boneArraysSize);

		std::vector<float> boneWeights;
		boneWeights.resize(boneArraysSize);

		for (int i = 0; i < mesh->mNumBones; i++)
		{
			aiBone* aiBone = mesh->mBones[i];

			for (int j = 0; j < aiBone->mNumWeights; j++)
			{
				aiVertexWeight weight = aiBone->mWeights[j];

				unsigned int vertexStart = weight.mVertexId * WEIGHTS_PER_VERTEX;

				for (int k = 0; k < WEIGHTS_PER_VERTEX; k++)
				{
					if (boneWeights.at(vertexStart + k) == 0)
					{
						boneWeights.at(vertexStart + k) = weight.mWeight;
						bonesIDs.at(vertexStart + k) = i;

					}
				}
			}
		}
		index = 0;
		int * indices = new int[mesh->mNumFaces * mesh->mFaces->mNumIndices];

		for (int f = 0; f < mesh->mNumFaces; f++)
		{
			aiFace face = mesh->mFaces[f];
			for (int ind = 0; ind < face.mNumIndices; ind++)
			{
				indices[index++] = face.mIndices[ind];
			}
		}
		
		for (int b = 0; b < mesh->mNumBones; b++)
		{
			aiBone * bone = mesh->mBones[b];

			for (int w = 0; w < bone->mNumWeights; w++)
			{
				aiVertexWeight weight = bone->mWeights[w];

				int vertexIndex = weight.mVertexId;
				int findex = vertexIndex * sizeOfVertex;
			}
		}
		
	}
	AnimationUtil();
	~AnimationUtil();
};

#endif // !1
