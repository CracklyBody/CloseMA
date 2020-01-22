#include "mesho.h"

mesh::mesh(std::vector<vertexData>* vd,std::vector<unsigned int>* id,std::vector<textureData>* td)
{
	data=*vd;
	numVertices = data.size();
	numIndices = id->size();
	indices=*id;
	if(td)
		textures=*td;
	
	std::vector<float> vecVerts;
	std::vector<float> vecNorms;
	std::vector<float> vecTangs;
	std::vector<float> vecCoords0;

	std::vector<float> vecWeights;
	std::vector<GLint> vecIDs;

	for (int i = 0; i < numVertices; i++)
	{
		vecVerts.push_back(data[i].position.x);
		vecVerts.push_back(data[i].position.y);
		vecVerts.push_back(data[i].position.z);

		vecNorms.push_back(data[i].normal.x);
		vecNorms.push_back(data[i].normal.y);
		vecNorms.push_back(data[i].normal.z);

		vecTangs.push_back(data[i].tangent.x);
		vecTangs.push_back(data[i].tangent.y);
		vecTangs.push_back(data[i].tangent.z);

		vecCoords0.push_back(data[i].U);
		vecCoords0.push_back(data[i].V);

		vecWeights.push_back(data[i].weight[0]);
		vecWeights.push_back(data[i].weight[1]);
		vecWeights.push_back(data[i].weight[2]);
		vecWeights.push_back(data[i].weight[3]);

		vecIDs.push_back(data[i].id[0]);
		vecIDs.push_back(data[i].id[1]);
		vecIDs.push_back(data[i].id[2]);
		vecIDs.push_back(data[i].id[3]);
	}

	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	glGenBuffers(1, &IND);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IND);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	
	glGenBuffers(1, &VBO_Pos);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Pos);

	glBufferData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(GLfloat), &vecVerts[0], GL_STATIC_DRAW);

	glGenBuffers(1, &VBO_UVs);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_UVs);

	glBufferData(GL_ARRAY_BUFFER, numVertices * 2 * sizeof(GLfloat), &vecCoords0[0], GL_STATIC_DRAW);

	glGenBuffers(1, &VBO_Nor);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Nor);

	glBufferData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(GLfloat), &vecNorms[0], GL_STATIC_DRAW);

	glGenBuffers(1, &VBO_Tan);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Tan);

	glBufferData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(GLfloat), &vecTangs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &VBO_IDs);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_IDs);

	glBufferData(GL_ARRAY_BUFFER, numVertices * 4 * sizeof(GLint), &vecIDs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &VBO_Weights);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Weights);

	glBufferData(GL_ARRAY_BUFFER, numVertices * 4 * sizeof(GLfloat), &vecWeights[0], GL_STATIC_DRAW);

	glGenBuffers(1, &indexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, indexBufferObject);

	glBufferData(GL_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	/*glGenBuffers(1,&VBO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,data.size()*sizeof(vertexData),
	&data[0],GL_STATIC_DRAW);
	
	
	glGenBuffers(1,&IND);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IND);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size()*sizeof(unsigned int),&indices[0],GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	*/
	
	glBindVertexArray(0);
}

mesh::~mesh()
{
	//glDeleteBuffers(1,&VBO);
	glDeleteBuffers(1,&IND);
}

void mesh::draw(unsigned int programId)
{
	glUseProgram(programId);


	// always good practice to set everything back to defaults once configured.
	
	glBindVertexArray(vertexArrayObject);
	//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Pos);
	unsigned int vpos = glGetAttribLocation(programId, "s_vPosition");
	glVertexAttribPointer(vpos, 3, GL_FLOAT, GL_FALSE,0,0);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO_UVs);
	unsigned int vcoords0 = glGetAttribLocation(programId, "s_vCoords0");
	glVertexAttribPointer(vcoords0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Nor);
	unsigned int vnorms = glGetAttribLocation(programId, "s_vNormals");
	glVertexAttribPointer(vnorms, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Tan);
	unsigned int vtangs = glGetAttribLocation(programId, "s_vTangents");
	glVertexAttribPointer(vtangs, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_IDs);
	unsigned int vbids = glGetAttribLocation(programId, "s_vIDs");
	glVertexAttribIPointer(vbids, 4, GL_INT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Weights);
	unsigned int vweights = glGetAttribLocation(programId, "s_vWeights");
	glVertexAttribPointer(vweights, 4, GL_FLOAT,GL_TRUE, 0, 0);

	glEnableVertexAttribArray(vpos);
	glEnableVertexAttribArray(vcoords0);
	glEnableVertexAttribArray(vnorms);
	glEnableVertexAttribArray(vtangs);

	glEnableVertexAttribArray(vbids);
	glEnableVertexAttribArray(vweights);


	// always good practice to set everything back to defaults once configured.
	//glActiveTexture(GL_TEXTURE0);
	
	//glBindVertexArray(0);

	////attribute vec3 vertex
	//int vertex=glGetAttribLocation(programId,"vertex"); //0
	//int normal=glGetAttribLocation(programId,"normal"); //1
	//int tangent=glGetAttribLocation(programId,"tangent"); //2
	//int color=glGetAttribLocation(programId,"color"); //3
	//int UV=glGetAttribLocation(programId,"UV"); //4
	//
	////texture0
	////texture1...
	//std::string str="texture";
	//for(int i=0;i<textures.size();i++)
	//{
	//	glActiveTexture(GL_TEXTURE0+i);
	//	glBindTexture(GL_TEXTURE_2D,textures[i].id);
	//	glUniform1i(glGetUniformLocation(programId,(str+(char)(i+'0')).c_str()),i);
	//}
	//
	//glBindBuffer(GL_ARRAY_BUFFER,VBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IND);
	//
	//glEnableVertexAttribArray(vertex);
	//glVertexAttribPointer(vertex,3,GL_FLOAT,GL_FALSE,sizeof(vertexData),0);
	//
	//glEnableVertexAttribArray(normal);
	//glVertexAttribPointer(normal,3,GL_FLOAT,GL_FALSE,sizeof(vertexData),(void*)(3*sizeof(float)));
	//
	//glEnableVertexAttribArray(tangent);
	//glVertexAttribPointer(tangent,3,GL_FLOAT,GL_FALSE,sizeof(vertexData),(void*)(6*sizeof(float)));
	//
	//glEnableVertexAttribArray(color);
	//glVertexAttribPointer(color,3,GL_FLOAT,GL_FALSE,sizeof(vertexData),(void*)(9*sizeof(float)));
	//
	//glEnableVertexAttribArray(UV);
	//glVertexAttribPointer(UV,2,GL_FLOAT,GL_FALSE,sizeof(vertexData),(void*)(12*sizeof(float)));
	//
	//glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,0);
	//
	//glDisableVertexAttribArray(vertex);
	//glDisableVertexAttribArray(normal);
	//glDisableVertexAttribArray(tangent);
	//glDisableVertexAttribArray(color);
	//glDisableVertexAttribArray(UV);
	//glBindBuffer(GL_ARRAY_BUFFER,0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);	
}
