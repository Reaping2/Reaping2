#version 330

uniform vec2 resolution;
uniform vec2 lightPosition;
uniform float heading;
uniform float aperture;
uniform float fsaperture;
uniform float distanceMult;
uniform float maxShadow;
uniform float lightSize;
uniform sampler2D texture;
smooth in vec2 inTexCoord;
out vec4 outputColor[2];
float pi = 3.141592654;
float angDiff( float facing, float target )
{
    return abs( mod( abs( pi + facing - target ), 2.0 * pi ) - pi );
}
bool isInCone( float facing, float target, float apert )
{
    return angDiff( facing, target ) < ( apert / 2.0 );
}
float coneScale( float facing, float target, float fsapert, float apert )
{
    float ad = angDiff( facing, target );
    return smoothstep( fsapert / 2.0, apert / 2.0, ad );
}
void main()
{
    vec2 uv = -1 * ( inTexCoord - lightPosition ) * 2.0;
    float theta = clamp( atan( uv.y, uv.x ) / pi * 0.5 + 0.5, 0, 1 );
    float rad = length( uv ) * distanceMult;
    uv.y *= resolution.y / resolution.x;
    float target = atan( uv.y, uv.x );
    float falloffDist = length( uv );
    float firstOccluder = texture2D( texture, vec2( theta, 0.5 ) ).r;
    if( !isInCone( heading + pi, target, aperture ) )
    {
        firstOccluder = 0; 
    }

    float distfactor = smoothstep( 0.15, 0.45, falloffDist );
    float shadow = distfactor;
    if( ( 1 > firstOccluder && firstOccluder < rad ) || lightSize <= 0.0 )
    {
        // first occluder is closer than this point
        // so shadow is the max castable
        shadow = max( shadow, 1.0 );
    }
    else
    {
        shadow = max( shadow, (distfactor + coneScale( heading + pi, target, fsaperture, aperture )) );
    }
    shadow = min( shadow, 1.0 );
    float alpha = 1.0 - shadow;
    outputColor[0] = vec4( alpha, alpha, alpha, 1 );
    // outputColor[0] = vec4( 1, 1, 1, 1 ) * alpha;
    alpha = 1.0 - shadow;
    outputColor[1] = vec4( alpha, alpha, alpha, 1 );
}
