#version 330

uniform sampler2D uiTexture;
out vec4 outputColor;
in vec2 inTexCoord;
in vec4 inColor;



void main(void)
{
	outputColor = inColor*texture2D(uiTexture,inTexCoord.st);
}

