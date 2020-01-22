#include "sceneLoader.h"

void meshLoader::recursiveProcess(aiNode* node,const aiScene* scene)
{
	//process
	for(int i=0;i<node->mNumMeshes;i++)
	{
		aiMesh* mesh=scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh,scene);
	}



	//recursion
	for(int i=0;i<node->mNumChildren;i++)
	{
		recursiveProcess(node->mChildren[i],scene);
	}
}

void meshLoader::processMesh(aiMesh* m,const aiScene* scene)
{
	// Определяем как много веса на вершину но так же как много индексов на вершину
	int WEIGHTS_PER_VERTEX = 4;
	// Первое что надо сделать это создать два массива они будут содержать все данные вершин
	int boneArraySize = m->mNumVertices * WEIGHTS_PER_VERTEX;
	// Это сделано потому что каждая вершина имеет место для 4=ёх весов и 4-ёх индексов
	std::vector<int> boneIDs;
	boneIDs.resize(boneArraySize);
	// Место для 4-ёх весов на вершину
	std::vector<float> boneWeights;
	boneWeights.resize(boneArraySize);

	std::vector<vertexData> data;
	std::vector<unsigned int> indices;
	std::vector<textureData> textures;
	aiColor4D col;
	aiMaterial* mat=scene->mMaterials[m->mMaterialIndex];
	aiGetMaterialColor(mat,AI_MATKEY_COLOR_DIFFUSE,&col);
	vector3d defaultColor(col.r,col.g,col.b);

	

	for(int i=0;i<m->mNumVertices;i++)
	{
			vertexData tmp;
			vector3d tmpVec;

			//position
			tmpVec.x=m->mVertices[i].x;
			tmpVec.y=m->mVertices[i].y;
			tmpVec.z=m->mVertices[i].z;
			tmp.position=tmpVec;

			//normals
			tmpVec.x=m->mNormals[i].x;
			tmpVec.y=m->mNormals[i].y;
			tmpVec.z=m->mNormals[i].z;
			tmp.normal=tmpVec;


			//tangent
			if(m->mTangents)
			{
				tmpVec.x=m->mTangents[i].x;
				tmpVec.y=m->mTangents[i].y;
				tmpVec.z=m->mTangents[i].z;
			}else{
				tmpVec.x=1.0;
				tmpVec.y=tmpVec.z=0;
			}
			tmp.tangent=tmpVec;


			//colors
			if(m->mColors[0])
			{
				//!= material color
				tmpVec.x=m->mColors[0][i].r;
				tmpVec.y=m->mColors[0][i].g;
				tmpVec.z=m->mColors[0][i].b;
			}else{
				tmpVec=defaultColor;
			}
			tmp.color=tmpVec;

			//color
			if(m->mTextureCoords[0])
			{
				tmpVec.x=m->mTextureCoords[0][i].x;
				tmpVec.y=m->mTextureCoords[0][i].y;
			}else{
				tmpVec.x=tmpVec.y=tmpVec.z=0.0;
			}
			tmp.U=tmpVec.x;
			tmp.V=tmpVec.y;
			data.push_back(tmp);
	}

	for(int i=0;i<m->mNumFaces;i++)
	{
		aiFace face=m->mFaces[i];
		for(int j=0;j<face.mNumIndices;j++) //0..2
		{
			indices.push_back(face.mIndices[j]);
		}
	}


	for(int i=0;i<mat->GetTextureCount(aiTextureType_DIFFUSE);i++)
	{
		aiString str;
		mat->GetTexture(aiTextureType_DIFFUSE,i,&str);
		textureData tmp;
		tmp.id=loadTexture(str.C_Str());
		tmp.type=0;
		textures.push_back(tmp);
	}
	// Тут мы заполняем массив
	// Мы делаем это циклически проходя каждую кость  и его данные
	// И копируем это в свой массив
	for (int i = 0; i < m->mNumBones; i++)
	{
		// Создаем указатель на текущую кость
		aiBone* aiBone = m->mBones[i];
		//Важная заметка то что i это просто ID для текущей кости

		for (int j = 0; j < aiBone->mNumWeights; j++)
		{
			aiVertexWeight weight = aiBone->mWeights[j];

			//Это будет говорить нам где в нашем массиве начинать читать вес вершин
			unsigned int vertexStart = weight.mVertexId * WEIGHTS_PER_VERTEX;

			// Здесь мы наконец заполняем массив обоими индексами и весами
			for (int k = 0; k < WEIGHTS_PER_VERTEX; k++)
			{
				// Если вес текущей кости равен 0 
				if (boneWeights.at(vertexStart + k) == 0)
				{
					// Тогда он не будет заполнен действительным весом
					boneWeights.at(vertexStart + k) = weight.mWeight;
					boneIDs.at(vertexStart + k) = i; //Запомнить i ЭТО просто ID текущей КОСТИ

					// Каждый класс vertexData имеет МЕСТО для ПОЗИЦИИ, UV, NORMAL, и 4 ИНДЕКСА, и 4 ВЕСА
					// Каждый mesh  создан с миссивом vertexData ( оно будет как data)
					data.at(weight.mVertexId).id[k] = i; // Задаем ID в k of the vertex at the weight's ID, to this weight's weight
					break;

				}
			}

		}
	}
	meshes.push_back(new mesh(&data,&indices,&textures));
}


unsigned int meshLoader::loadTexture(const char* filename)
{
	unsigned int num;
	glGenTextures(1,&num);
	//SDL_Surface* img=IMG_Load(filename);
	//if(img==NULL)
	//{
	//	//std::cout << "img was not loaded" << std::endl;
	//	return -1;
	//}
	//SDL_PixelFormat form={NULL,32,4,0,0,0,0,0,0,0,0,0xff000000,0x00ff0000,0x0000ff00,0x000000ff,0,255};
	//SDL_Surface* img2=SDL_ConvertSurface(img,&form,SDL_SWSURFACE);
	//if(img2==NULL)
	//{
	//	//std::cout << "img2 was not loaded" << std::endl;
	//	return -1;
	//}
	glBindTexture(GL_TEXTURE_2D,num);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	//glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img2->w,img2->h,0,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,img2->pixels);
	//SDL_FreeSurface(img);
	//SDL_FreeSurface(img2);
	//return num;
	return 0;
}


meshLoader::meshLoader(const char* filename)
{
	const char* s;
	Assimp::Importer importer;
	const aiScene* scene=importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);//aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_FlipUVs);
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "The file wasn't successfuly opened " << filename << std::endl;
		return;
	}

	recursiveProcess(scene->mRootNode,scene);
}

meshLoader::~meshLoader()
{
	for(int i=0;i<meshes.size();i++)
		delete meshes[i];
}

void meshLoader::draw(unsigned int programId)
{
	for(int i=0;i<meshes.size();i++)
		meshes[i]->draw(programId);
}

std::vector<mesh*>& meshLoader::getMeshes()
{
	return meshes;
}
