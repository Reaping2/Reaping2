#version 330

uniform sampler2D texture;
uniform float maxShadow;
smooth in vec2 inTexCoord;
out vec4 outputColor;
void main()
{
    vec4 col = texture2D(texture, inTexCoord);
    outputColor = vec4( 0,0,0, maxShadow * min( 1.0 - col.r, col.a ) );
}
