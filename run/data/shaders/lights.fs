#version 330

uniform vec2 resolution;
uniform vec2 lightPosition;
uniform float distanceMult;
uniform float maxShadow;
uniform sampler2D texture;
smooth in vec2 inTexCoord;
out vec4 outputColor;
void main()
{
    vec2 uv = -1 * ( inTexCoord - lightPosition ) * 2.0;
    float theta = clamp( atan( uv.y, uv.x ) / 3.14159 * 0.5 + 0.5, 0, 1 );
    float rad = length( uv ) * distanceMult;
    uv.y *= resolution.y / resolution.x;
    float falloffDist = length( uv );
    float firstOccluder = texture2D( texture, vec2( theta, 0.5 ) ).r;
    float alpha = smoothstep( 0.15, 0.45, falloffDist ) * maxShadow;
    if( 1 > firstOccluder && firstOccluder < rad )
    {
        alpha = max( alpha, maxShadow );
    }
    alpha = 1.0 - alpha;
    outputColor = vec4( 1, 1, 1, 1 ) * alpha;
}
