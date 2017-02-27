#version 330

uniform sampler2D texture;
uniform sampler2D secondaryTexture;
uniform sampler2D lightTexture;
uniform vec2 lightPosition;
out vec4 outputColor;
smooth in vec2 inTexCoord;

uniform float time;
uniform float lightSize;
uniform vec2 resolution;

float lightZ = 0.075;
vec4 lightColor = vec4( 1, 0.8, 0.6, 1 );
vec4 ambientColor = vec4( 1.0, 1.0, 1.0, 0.7 );
vec3 falloff = vec3( 0.15, 2.7, 14 );

void main(void)
{
    vec4 normalColor = texture2D( secondaryTexture, inTexCoord );
    vec4 color = texture2D( texture, inTexCoord );
    vec3 ambient = ambientColor.rgb * ambientColor.a;
    if( lightSize <= 0.0 )
    {
        outputColor = vec4(color.rgb * ambient, color.a);
        return;
    }
    vec2 lightCoord = lightPosition;
    vec3 lightDir = vec3( lightCoord - inTexCoord, lightZ );
    lightDir.x *= resolution.x / resolution.y;
    float D = length( lightDir );

    vec4 lighttcol = texture2D( lightTexture, inTexCoord );

    vec3 normal = vec3( normalColor.r, 1.0 - normalColor.g, normalColor.b ) * 2.0 - vec3( 1,1,1 );

    vec3 N = normalize( normal );
    vec3 L = normalize( lightDir );

    vec3 diffuse = (lightColor.rgb * lightColor.a) * max( dot( N, L ), 0.0 );

    float attenuation = 1.0 / ( falloff.x + falloff.y * D + falloff.z * D * D );

    vec3 intensity = ambient + diffuse * lighttcol.rgb * attenuation * lighttcol.a;
    vec3 finalColor = color.rgb * intensity;

    outputColor = vec4( finalColor, color.a );
}

