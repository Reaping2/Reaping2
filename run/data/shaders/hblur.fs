#version 330

uniform sampler2D texture;

smooth in vec2 inTexCoord;
smooth in vec2 blurTexCoords[14];
out vec4 outputColor;

void main()
{
    outputColor = vec4(0.0);
    outputColor += texture2D(texture, blurTexCoords[ 0])*0.0044299121055113265 *2;
    outputColor += texture2D(texture, blurTexCoords[ 1])*0.00895781211794 *2;
    outputColor += texture2D(texture, blurTexCoords[ 2])*0.0215963866053 *2;
    outputColor += texture2D(texture, blurTexCoords[ 3])*0.0443683338718 *2;
    outputColor += texture2D(texture, blurTexCoords[ 4])*0.0776744219933 *2;
    outputColor += texture2D(texture, blurTexCoords[ 5])*0.115876621105 *2;
    outputColor += texture2D(texture, blurTexCoords[ 6])*0.147308056121 *2;
    outputColor += texture2D(texture, inTexCoord       )*0.159576912161;
/*    outputColor += texture2D(texture, blurTexCoords[ 7])*0.147308056121;
    outputColor += texture2D(texture, blurTexCoords[ 8])*0.115876621105;
    outputColor += texture2D(texture, blurTexCoords[ 9])*0.0776744219933;
    outputColor += texture2D(texture, blurTexCoords[10])*0.0443683338718;
    outputColor += texture2D(texture, blurTexCoords[11])*0.0215963866053;
    outputColor += texture2D(texture, blurTexCoords[12])*0.00895781211794;
    outputColor += texture2D(texture, blurTexCoords[13])*0.0044299121055113265;*/
    // outputColor *= 2.0;
}
