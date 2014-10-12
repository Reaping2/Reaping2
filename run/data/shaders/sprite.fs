#version 330

uniform sampler2D spriteTexture;
smooth in vec2 inTexCoord;
out vec4 outputColor;
void main()
{
	outputColor = texture2D(spriteTexture,inTexCoord.st);
}
