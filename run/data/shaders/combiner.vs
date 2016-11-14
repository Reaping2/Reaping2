#version 330

//---global uniforms
layout(std140) uniform GlobalMatrices
{
    mat4 cameraToClipMatrix;
    mat4 worldToCameraMatrix;
};

//---shader spec
layout(location=0) in vec2 texcoord;
layout(location=1) in vec2 position;
smooth out vec2 inTexCoord;
void main()
{
    inTexCoord=texcoord;
    gl_Position=vec4( 2 * ( position - vec2( 0.5, 0.5 ) ) ,0,1);
}
