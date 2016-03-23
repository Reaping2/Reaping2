#version 330

uniform sampler2D texture;
smooth in vec2 inTexCoord;
out vec4 outputColor;
const vec2 resolution = vec2( 1920.0, 1080.0 );
const vec2 mouse = vec2( 200.0, 200.0 );
const float lensSize = 0.1;
void main()
{
    vec2 p = inTexCoord;
    vec2 m = mouse.xy / resolution.xy;

    vec2 d = p - m;
    float r = sqrt(dot(d, d));

    vec3 col = vec3(0.0, 0.0, 0.0);
    if( r > lensSize + 0.01 )
    {
        col = texture2D(texture, p).xyz;
    }
    else
    {
        vec2 uv = m + d * r / lensSize;
        col = texture2D(texture, uv ).xyz;
    }
    outputColor = vec4(col,1);
}
