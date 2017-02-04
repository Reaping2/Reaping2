#version 330

uniform sampler2D texture;
uniform sampler2D secondaryTexture;
out vec4 outputColor;
smooth in vec2 inTexCoord;

uniform float time;
uniform vec2 resolution;

const float uFreqY = 320.;
const float uFreqX = 100.;
const float uSpeed = 4.;
const float uAmplitude = 0.00075;

void main(void)
{
    vec2 uv = inTexCoord;
    vec2 disp = vec2(sin(uFreqY*uv.y + uFreqX*uv.x + uSpeed*time) * uAmplitude);
    vec4 col = texture2D(texture,uv );
    float secColor = 1.0;
    for( int i = -3; i <=3 && secColor > 0.1; ++i )
    {
        for( int j = -3; j <=3 && secColor > 0.1; ++j )
        {
             secColor *= texture2D(secondaryTexture, (gl_FragCoord.xy + 1 * vec2(i,j)) / resolution.xy).r;
        }
    }
    if( secColor <= 0.11 )
    {
        secColor = 0.0;
    }
    else
    {
        secColor = 1.0;
    }
    outputColor = texture2D(texture,uv + disp * secColor) * vec4( 1,1,1,secColor );
}

