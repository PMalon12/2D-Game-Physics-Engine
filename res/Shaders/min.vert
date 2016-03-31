#version 420

layout(binding = 0) uniform Mat
{
	mat4 proj;
};

uniform mat4 model;

in vec3 position;

void main() {
   gl_Position = proj * model * vec4(position.x, position.y, position.z, 1.f);
};