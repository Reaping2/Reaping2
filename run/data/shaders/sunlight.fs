#version 330

uniform vec2 resolution;
uniform sampler2D texture;
smooth in vec2 inTexCoord;
uniform float maxShadow;
out vec4 outputColor;
void main()
{
    vec2 p = inTexCoord;
    float alpha = 0.0;
    for( int i = 1, e = 5; i < e; ++i )
    {
        vec4 c = texture2D( texture, vec2( p.x - i / resolution.x, p.y + i  / resolution.y ) );
        alpha = max( alpha, c.w / ( 1.0 * i ) );
        if( alpha > 0.0 )
        {
            alpha = clamp( e / 2.0 / ( 1. + i ), 0.35, 0.85 );
            break;
        }
    }
    alpha = min( alpha, maxShadow );
    alpha = 1.0 - alpha;
    outputColor = vec4( 1, 1, 1, 1 ) * 0.85 * alpha;
}
