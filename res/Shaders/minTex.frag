#version 420
in vec2 Texcoord;
out vec4 outColour;

uniform vec4 colour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
uniform sampler2D sampTex;

void main() 
{
	vec4 tex = texture(sampTex, Texcoord);

	/*tex += texture(sampTex, Texcoord + vec2(0.005,0.0))* 0.1;
	tex += texture(sampTex, Texcoord + vec2(0.010,0.0))* 0.1;
	tex += texture(sampTex, Texcoord + vec2(0.015,0.0))* 0.1;
	tex += texture(sampTex, Texcoord + vec2(0.020,0.0))* 0.1;
	tex += texture(sampTex, Texcoord + vec2(0.025,0.0))* 0.1;
	tex += texture(sampTex, Texcoord + vec2(0.030,0.0))* 0.1;

	tex += texture(sampTex, Texcoord - vec2(0.005,0.0))* 0.1;
	tex += texture(sampTex, Texcoord - vec2(0.010,0.0))* 0.1;
	tex += texture(sampTex, Texcoord - vec2(0.015,0.0))* 0.1;
	tex += texture(sampTex, Texcoord - vec2(0.020,0.0))* 0.1;
	tex += texture(sampTex, Texcoord - vec2(0.025,0.0))* 0.1;
	tex += texture(sampTex, Texcoord - vec2(0.030,0.0))* 0.1;

	tex += texture(sampTex, Texcoord + vec2(0.0,0.005))* 0.1;
	tex += texture(sampTex, Texcoord + vec2(0.0,0.010))* 0.1;
	tex += texture(sampTex, Texcoord + vec2(0.0,0.015))* 0.1;
	tex += texture(sampTex, Texcoord + vec2(0.0,0.020))* 0.1;
	tex += texture(sampTex, Texcoord + vec2(0.0,0.025))* 0.1;
	tex += texture(sampTex, Texcoord + vec2(0.0,0.030))* 0.1;

	tex += texture(sampTex, Texcoord - vec2(0.0,0.005))* 0.1;
	tex += texture(sampTex, Texcoord - vec2(0.0,0.010))* 0.1;
	tex += texture(sampTex, Texcoord - vec2(0.0,0.015))* 0.1;
	tex += texture(sampTex, Texcoord - vec2(0.0,0.020))* 0.1;
	tex += texture(sampTex, Texcoord - vec2(0.0,0.025))* 0.1;
	tex += texture(sampTex, Texcoord - vec2(0.0,0.030))* 0.1;*/

	//tex *= vec4(0.3,0.3,0.3,1.0);

	outColour =  colour * tex;
}