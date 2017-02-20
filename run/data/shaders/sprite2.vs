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

uniform vec2 lightVec;

vec2[4] corners=vec2[](
        vec2(-1,-1)
        ,vec2(1,-1)
        ,vec2(-1,1)
        ,vec2(1,1)
);

int[4] ucord=int[](
    0
    ,1
    ,0
    ,1
);

int[4] vcord=int[](
    2
    ,2
    ,3
    ,3
);

void main()
{
    int vid = int(floor(mod(gl_VertexID,4.0)));
    inTexCoord=vec2(TexCoord[ucord[vid]],TexCoord[vcord[vid]]);
    inNormalTexCoord=vec2(normalTexCoord[ucord[vid]],normalTexCoord[vcord[vid]]);
    inColor=color;
    vec2 position=corners[vid];
    position.x *= Size.x;
    position.y *= Size.y;
    mat2 ScaleMatrix=mat2(cos(Heading),sin(Heading),-sin(Heading),cos(Heading));
    float numsteps=ceil(max(abs(lightVec.x),abs(lightVec.y))/30.0);
    vec2 displace=vec2(0,0);
    if( numsteps > 0.5 && gl_VertexID >= 4 )
    {
        displace = lightVec / numsteps * ( 1.0 + floor(gl_VertexID/4.0) );
    }
    gl_Position=cameraToClipMatrix*worldToCameraMatrix*vec4(ScaleMatrix*position+SpriteCenter+displace,0,1);
}
