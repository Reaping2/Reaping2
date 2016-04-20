#version 330

uniform vec2 resolution;
uniform sampler2D texture;
smooth in vec2 inTexCoord;
out vec4 outputColor;
void main()
{
    vec2 p = inTexCoord;
    float alpha = 0.0;
    for( int i = 1; i < 11; ++i )
    {
        vec4 c = texture2D( texture, vec2( p.x - i / resolution.x, p.y + i  / resolution.y ) );
        alpha = max( alpha, c.w / ( 1.0 * i ) );
        if( alpha > 0.0 )
        {
            alpha = clamp( 5.0 / ( 1. + i ), 0.15, 0.85 );
            break;
        }
    }
    outputColor = vec4( 0, 0, 0, alpha );
}
