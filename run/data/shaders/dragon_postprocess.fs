#version 330

uniform sampler2D texture;
uniform sampler2D secondaryTexture;
out vec4 outputColor;
smooth in vec2 inTexCoord;
smooth in mat4 inUnproject;
smooth in vec2 inResolution;

uniform float time;
uniform vec2 resolution;

const float uFreqY = 320.;
const float uFreqX = 100.;
const float uSpeed = 4.;
const float uAmplitude = 0.00075;

vec4 fun( float t, vec2 worldpos )
{
    float red = clamp( sin( t ) * cos( t ) * 10, 0, 1 );
    return vec4( red, red / 10, red / 10, 1 );
}

void main(void)
{
    vec4 orig = inUnproject * vec4( 2 * gl_FragCoord.xy / inResolution - 1,1,1 );

    vec2 uv = inTexCoord;

    vec4 col = texture2D(texture,uv );
    float secColor = texture2D(secondaryTexture, uv).r;
    outputColor = col * fun( time, orig.xy )  * vec4( 1,1,1,secColor );
}

