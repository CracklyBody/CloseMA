#version 140

//uniform mat4 MMatrix;
//uniform mat4 VPMatrix;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec3 s_vPosition;
in vec2 s_vCoords0;

in vec3 s_vNormals;
in vec3 s_vTangents;

out vec2 coords0;

const int MAX_BONES = 100;
;uniform mat4 gBones[MAX_BONES];

in vec4 s_vWeights;
in ivec4 s_vIDs;

out vec4 we;
out vec4 id;

void main ()
{
     // gl_Position = VPMatrix * MMatrix * (vec4(s_vPosition,1.0));
	gl_Position = projection * view * model *(vec4(s_vPosition,1.0));

      we = s_vWeights;
      id = s_vIDs;
 
      coords0 = s_vCoords0.xy;
}