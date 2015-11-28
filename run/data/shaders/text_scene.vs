#version 330

//---global uniforms
layout(std140) uniform GlobalMatrices
{
	mat4 cameraToClipMatrix;
	mat4 worldToCameraMatrix;
};

//---shader spec
layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec4 color;
layout(location = 3) in vec2 TextPosition;
//layout(location = 4) in float Radius;

out vec2 inTexCoord;
out vec4 inColor;
void main()
{
	inTexCoord=texcoord;
	inColor=color;
	mat2 ScaleMatrix=mat2(1.0);
	gl_Position = cameraToClipMatrix*worldToCameraMatrix* vec4(ScaleMatrix*position+TextPosition,0,1);
}
