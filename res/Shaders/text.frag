#version 420

uniform sampler2D sampTex;

in vec4 Colour;
in vec2 Texcoord;

out vec4 outColour;

void main() {
	vec4 tex = texture(sampTex, Texcoord);
	outColour = vec4(Colour.rgb,Colour.a*tex.a);
}