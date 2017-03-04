#version 330

uniform sampler2D texture;
out vec4 outputColor;
smooth in vec2 inTexCoord;

uniform float time;
uniform vec2 resolution;

vec4 ambientColor = vec4( 1.0, 1.0, 1.0, 0.7 );

void main(void)
{
    vec4 color = texture2D( texture, inTexCoord );
    vec3 ambient = ambientColor.rgb * ambientColor.a;

    outputColor = vec4(color.rgb * ambient, color.a);
}

