#ifndef MESHO_H
#define MESHO_H
#include <iostream>
//#include "GLee.h"
#include "meshData.h"
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <glad/glad.h>
#include <vector>
#include <string>

class mesh{
	std::vector<vertexData> data;
	std::vector<textureData> textures;
	std::vector<unsigned int> indices;
	unsigned int vertexArrayObject;
	unsigned int numVertices;
	unsigned int numIndices;
	unsigned int VBO_Pos;
	unsigned int VBO_UVs;
	unsigned int VBO_Nor;
	unsigned int VBO_Tan;
	unsigned int VBO_IDs;
	unsigned int VBO_Weights;
	unsigned int indexBufferObject;
	unsigned int IND;
	public:
		mesh(std::vector<vertexData>* vd,std::vector<unsigned int>* id,std::vector<textureData>* td=NULL);
		~mesh();
		void draw(unsigned int programId);
};
#endif
