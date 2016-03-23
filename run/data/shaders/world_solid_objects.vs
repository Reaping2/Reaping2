#version 330

//---global uniforms
layout(std140) uniform GlobalMatrices
{
    mat4 cameraToClipMatrix;
    mat4 worldToCameraMatrix;
};

//---shader spec
layout(location=0) in vec2 position;
layout(location=1) in vec2 texcoord;
smooth out vec2 inTexCoord;
void main()
{
    inTexCoord=texcoord;
    gl_Position=vec4(position,0,1);
}
