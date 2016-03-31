#version 420

out vec4 outColour;
uniform vec4 colour = vec4(1.0f, 0.0f, 1.0f, 0.8f);
void main() {
	outColour = colour;
}

