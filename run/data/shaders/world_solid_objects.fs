#version 330

uniform sampler2D texture;
smooth in vec2 inTexCoord;
smooth in mat4 unproject;
out vec4 outputColor;
void main()
{
    vec4 orig = unproject * gl_FragCoord;
    vec4 col = vec4( 1,1,1,1 );
    if( orig.x < 0 )
    {
//        col.x = 0;
    }
    outputColor = texture2D(texture, inTexCoord) * col;
}
