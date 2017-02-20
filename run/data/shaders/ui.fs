#version 330

uniform sampler2D uiTexture;
out vec4 outputColor;
in vec2 inTexCoord;
in vec4 inColor;

uniform float time;
uniform vec2 resolution;


void main(void)
{
	float t = time * 1.0/300.0;
	vec2 p = 10.0*(( gl_FragCoord.xy / resolution.xy )-1.5-vec2(t));
	p.x*=resolution.x/resolution.y;
	float color = 0.0;
	mat2 rot=mat2(cos(t),sin(t),-sin(t),cos(t));
	for(int i=0;i<4;++i){
		p+=vec2(10.0*sin(9.0*t),0);
		p*=rot;
		color+=cos(p.x-2.0)*cos(p.x-2.0)*sin(p.y-2.0)*sin(p.y-2.0);
	}
	color=fract(abs(color));
	color*=1.0-color;
	color=1.0-color;
	color*=color;

	outputColor = inColor * ((inTexCoord.s<0.0)?vec4(1,1,1,1):texture2D(uiTexture,inTexCoord.st));// * vec4(color,color,color,1.0);
}

