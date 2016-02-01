#version 330

uniform sampler2D texture;
smooth in vec2 inTexCoord;
out vec4 outputColor;
void main()
{
    outputColor = vec4(texture2D(texture,inTexCoord.st).xyz,1);
}
