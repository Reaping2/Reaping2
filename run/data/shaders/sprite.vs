#version 330

//---global uniforms
layout(std140) uniform GlobalMatrices
{
	mat4 cameraToClipMatrix;
	mat4 worldToCameraMatrix;
};

//---shader spec
uniform vec2 spriteCenter;
uniform mat2 scaleMatrix;

layout(location=0) in vec4 position;
layout(location=1) in vec2 TexCoord;
smooth out vec2 inTexCoord;
void main()
{
	inTexCoord=TexCoord;
	gl_Position=cameraToClipMatrix*worldToCameraMatrix*vec4(scaleMatrix*position.xy+spriteCenter,0,1);
}
