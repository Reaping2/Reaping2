#version 330

uniform sampler2D spriteTexture;
uniform sampler2D secondarySpriteTexture;
out vec4 outputColor;
smooth in vec2 inTexCoord;
smooth in vec2 inSecondaryTexCoord;
smooth in vec4 inColor;
in vec4 limits;

uniform float time;
uniform vec2 resolution;


void main(void)
{
    float t = time;
    mat2 rot=mat2(cos(t),sin(t),-sin(t),cos(t));
    vec2 range = ( limits.zw - limits.xy ) / 10.0;
    vec2 inp = inSecondaryTexCoord;
    vec2 sec = limits.xy + rot * (inp-limits.xy);
    sec = inp - range / 2 + rot * range;
    sec = clamp( sec, limits.xy, limits.zw );
    vec4 disp = texture2D( secondarySpriteTexture, inSecondaryTexCoord  + fract( t * vec2( 0.2, 0.4 ) ) );

    outputColor = inColor * texture2D(spriteTexture,inTexCoord + range * disp.xy );
}

