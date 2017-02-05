#version 330

uniform vec2 resolution;
uniform sampler2D texture;
smooth in vec2 inTexCoord;
out vec4 outputColor;
const float maxshadow = 0.6;
void main()
{
    vec2 uv = -1 * ( inTexCoord * 2.0 - 1 );
    float theta = clamp( atan( uv.y, uv.x ) / 3.14159 * 0.5 + 0.5, 0, 1 );
    float rad = length( uv );
    float firstOccluder = texture2D( texture, vec2( theta, 0.5 ) ).r;
    float alpha = smoothstep( 0.3, 0.9, rad ) * maxshadow;
    if( firstOccluder < rad )
    {
        alpha = max( alpha, maxshadow );
    }
    outputColor = vec4( 0, 0, 0, alpha );
}
