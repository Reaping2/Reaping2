#version 330

uniform sampler2D sceneTexture;
smooth in vec2 inTexCoord;
layout( location = 0 ) out vec4 outputColor;
void main()
{
    outputColor = vec4(texture2D(sceneTexture,inTexCoord.st).xyz,1);
}
