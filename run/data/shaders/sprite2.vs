#version 330

//---global uniforms
layout(std140) uniform GlobalMatrices
{
    mat4 cameraToClipMatrix;
    mat4 worldToCameraMatrix;
    mat4 inverseProjection;
    vec2 resolution;
};

//---shader spec
layout(location=0) in vec4 TexCoord;
layout(location=1) in vec4 SecondaryTexCoord;
layout(location=2) in vec2 SpriteCenter;
layout(location=3) in float Heading;
layout(location=4) in vec2 Size;
layout(location=5) in vec4 color;
layout(location=6) in vec4 procTexCoord;
layout(location=7) in vec4 normalTexCoord;
layout(location=8) in vec4 proccolor;
smooth out vec2 inTexCoord;
smooth out vec4 inColor;
smooth out vec2 inNormalTexCoord;
vec2[4] corners=vec2[4](vec2(-1,-1),
        vec2(1,-1),
        vec2(-1,1),
        vec2(1,1));
void main()
{
    inTexCoord=vec2(TexCoord[int(floor(mod(gl_VertexID,2.0)))],TexCoord[2+int(floor(gl_VertexID/2.0))]);
    inNormalTexCoord=vec2(normalTexCoord[int(floor(mod(gl_VertexID,2.0)))],normalTexCoord[2+int(floor(gl_VertexID/2.0))]);
    inColor=color;
    vec2 position=corners[gl_VertexID];
    position.x *= Size.x;
    position.y *= Size.y;
    mat2 ScaleMatrix=mat2(cos(Heading),sin(Heading),-sin(Heading),cos(Heading));
    gl_Position=cameraToClipMatrix*worldToCameraMatrix*vec4(ScaleMatrix*position+SpriteCenter,0,1);
}
