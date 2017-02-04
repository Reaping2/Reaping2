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
    float baseoffset = trunc( abs( worldpos.x / 11 ) ) * 11
        + trunc( abs( worldpos.x / 13 ) ) * 13;
    float speed = mod( trunc( (abs( worldpos.x ) + abs( worldpos.y )) / 23 )* 10, 19) / 19;
    float redpulse = clamp( sin( baseoffset + t * speed ) * 4, 0, 1 );
    vec4 col = vec4( 0.8, cos(t)*0.4, sin(t)*0.3, redpulse );
    return vec4( col.rgba );
}

void main(void)
{
    vec4 orig = inUnproject * vec4( 2 * gl_FragCoord.xy / inResolution - 1,1,1 );

    vec2 uv = inTexCoord;

    vec4 col = texture2D(texture,uv );
    float secColor = texture2D(secondaryTexture, uv).r;
    outputColor = col * fun( time, orig.xy ) * vec4( 1,1,1,secColor );
}

