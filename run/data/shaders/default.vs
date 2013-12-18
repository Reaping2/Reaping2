#version 330

//---global uniforms
layout(std140) uniform GlobalMatrices
{
	mat4 cameraToClipMatrix;
	mat4 worldToCameraMatrix;
};

//---shader spec
layout(location = 0) in vec4 position;
void main()
{
	gl_Position = position;
}
