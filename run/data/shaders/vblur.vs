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
smooth out vec2 blurTexCoords[14];
void main()
{
    inTexCoord=texcoord;
    gl_Position=vec4(position,0,1);
    blurTexCoords[ 0] = texcoord + vec2(0.0, -0.028);
    blurTexCoords[ 1] = texcoord + vec2(0.0, -0.024);
    blurTexCoords[ 2] = texcoord + vec2(0.0, -0.020);
    blurTexCoords[ 3] = texcoord + vec2(0.0, -0.016);
    blurTexCoords[ 4] = texcoord + vec2(0.0, -0.012);
    blurTexCoords[ 5] = texcoord + vec2(0.0, -0.008);
    blurTexCoords[ 6] = texcoord + vec2(0.0, -0.004);
    blurTexCoords[ 7] = texcoord + vec2(0.0,  0.004);
    blurTexCoords[ 8] = texcoord + vec2(0.0,  0.008);
    blurTexCoords[ 9] = texcoord + vec2(0.0,  0.012);
    blurTexCoords[10] = texcoord + vec2(0.0,  0.016);
    blurTexCoords[11] = texcoord + vec2(0.0,  0.020);
    blurTexCoords[12] = texcoord + vec2(0.0,  0.024);
    blurTexCoords[13] = texcoord + vec2(0.0,  0.028);
}

