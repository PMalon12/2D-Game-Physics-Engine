#version 420

layout(binding = 0) uniform Mat
{
	mat4 proj;
};

in vec3 position;

void main() {
   gl_Position = proj * vec4(position.x, position.y, position.z, 1.0);
};