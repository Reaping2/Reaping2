#version 330

layout(std140) uniform GlobalMatrices
{
	mat4 cameraToClipMatrix;
	mat4 worldToCameraMatrix;
};

out vec4 outputColor;
in vec2 inPosition;
in vec4 inColor;

void main(void)
{
	outputColor = inColor;
}

