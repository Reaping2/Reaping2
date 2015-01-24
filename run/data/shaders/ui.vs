#version 330

//---shader spec
layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec4 color;
uniform mat4 uiProjection;

out vec2 inTexCoord;
out vec4 inColor;
void main()
{
	inTexCoord=texcoord;
	inColor=color;
	gl_Position = uiProjection * vec4(position,0,1);
}
