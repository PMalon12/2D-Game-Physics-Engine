#version 420

layout(binding = 0) uniform Mat
{
	mat4 proj;
};

uniform mat4 model;

in vec3 position;
in vec2 texCoord;
in vec4 colour;

out vec2 Texcoord;
out vec4 Colour;

void main() {
	Colour = colour;
	Texcoord = texCoord;
	gl_Position = proj * model * vec4(position.x, position.y, 0.0, 1.0);
	//gl_Position = vec4(position.x - 1.0f, 1.0f + position.y, position.z, 1.0);
};