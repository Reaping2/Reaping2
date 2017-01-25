#version 330

uniform sampler2D spriteTexture;
smooth in vec2 inTexCoord;
smooth in vec4 inColor;
layout( location = 0 ) out vec4 outputColor;
void main()
{
    float alpha = texture2D(spriteTexture,inTexCoord.st).a;
    outputColor = vec4( inColor.rgb * alpha, alpha);
}
