#version 330

uniform sampler2D texture;
uniform sampler2D secondaryTexture;
out vec4 outputColor;
smooth in vec2 inTexCoord;

uniform float time;
uniform vec2 resolution;

float lightZ = 0.075;
vec4 lightColor = vec4( 1, 0.8, 0.6, 1 );
vec4 ambientColor = vec4( 1.0, 1.0, 1.0, 0.7 );
vec3 falloff = vec3( 0.15, 2.7, 14 );

void main(void)
{
    vec2 lightCoord = vec2( 0.5, 0.5 );
    vec3 lightDir = vec3( lightCoord - gl_FragCoord.xy / resolution, lightZ );
    lightDir.x *= resolution.x / resolution.y;
    float D = length( lightDir );

    vec4 color = texture2D( texture, inTexCoord );
    vec4 normalColor = texture2D( secondaryTexture, inTexCoord );
    vec3 normal = normalColor.rgb * 2.0 - vec3( 1,1,1 );

    vec3 N = normalize( normal );
    vec3 L = normalize( lightDir );

    vec3 diffuse = (lightColor.rgb * lightColor.a) * max( dot( N, L ), 0.0 );
    vec3 ambient = ambientColor.rgb * ambientColor.a;

    float attenuation = 1.0 / ( falloff.x + falloff.y * D + falloff.z * D * D );

    vec3 intensity = ambient + diffuse * attenuation;
    vec3 finalColor = color.rgb * intensity;

    if( normalColor.x == 1 &&
        normalColor.y == 1 &&
        normalColor.z == 1 )
    {
        outputColor = color;
    }
    else
    {
        outputColor = vec4( finalColor, color.a );
    }
}

