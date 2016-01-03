#version 330

uniform sampler2D spriteTexture;
smooth in vec2 inTexCoord;
smooth in vec4 inColor;
out vec4 outputColor;
void main()
{
    outputColor = texture2D(spriteTexture,inTexCoord);
    outputColor = vec4( outputColor.x * inColor.x, outputColor.y * inColor.y, outputColor.z * inColor.z, outputColor.w * inColor.w );
}
