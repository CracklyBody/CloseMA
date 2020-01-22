#version 140
//uniform mat4 MMatrix;
//uniform mat4 VPMatrix;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec2 coords0;
in vec4 we;
in vec4 id;

out vec4 fColor;

uniform sampler2D texUnit;

void main ()
{
      //vec4 texcolor = texture2D(texUnit,coords0);
      vec4 weightsColor = vec4(we.xyz,1.0);
 
      fColor = weightsColor;
}