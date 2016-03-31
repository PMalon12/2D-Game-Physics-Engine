#version 420

layout(binding = 0) uniform Mat
{
	mat4 proj;
};

uniform mat4 model;

in vec2 position;

void main() {
   gl_Position = proj * model * vec4(position.x, position.y, 0.f, 1.f);
};