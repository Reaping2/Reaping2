#version 330

//---global uniforms
layout(std140) uniform GlobalMatrices
{
	mat4 cameraToClipMatrix;
	mat4 worldToCameraMatrix;
};

//---shader spec
layout(location = 0) in vec2 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 TextPosition;

out vec2 inPosition;
out vec4 inColor;
void main()
{
	//inTexCoord=texcoord;
	inPosition=TextPosition;
	inColor=color;
	gl_Position = cameraToClipMatrix*worldToCameraMatrix* vec4(position,0,1);
}
