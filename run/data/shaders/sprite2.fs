#version 330

uniform sampler2D spriteTexture;
uniform sampler2D normalTexture;
smooth in vec2 inTexCoord;
smooth in vec4 inColor;
smooth in vec2 inNormalTexCoord;
out vec4 outputColor[2];
void main()
{
    vec4 col = inColor * texture2D(spriteTexture,inTexCoord);
    vec4 col2 = vec4( 1, 1, 1, col.a );
    if( inNormalTexCoord.x > -0.5 )
    {
        col2 = vec4( 1, 1, 1, col.a ) *
            texture2D( normalTexture, inNormalTexCoord );
    }
    outputColor[0] = col;
    outputColor[1] = col2;
}
