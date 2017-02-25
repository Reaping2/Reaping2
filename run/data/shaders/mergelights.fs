#version 330

uniform sampler2D texture;
uniform float maxShadow;
smooth in vec2 inTexCoord;
out vec4 outputColor;
void main()
{
    // color of the lightmap on lightrl ( single shadow level, all lights )
    vec4 col = texture2D(texture, inTexCoord);
    outputColor = col;
}
