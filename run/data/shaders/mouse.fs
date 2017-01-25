#version 330

uniform sampler2D spriteTexture;
smooth in vec2 inTexCoord;
smooth in vec4 inColor;
layout( location = 0 ) out vec4 outputColor;
void main()
{
    outputColor = inColor * texture2D(spriteTexture,inTexCoord.st);
}
