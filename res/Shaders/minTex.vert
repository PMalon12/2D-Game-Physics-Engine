#version 420

layout(binding = 0) uniform Mat
{
	mat4 proj;
};

uniform mat4 model;
uniform vec2 texCoordOffset = vec2(0.f, 0.f);

in vec3 position;
in vec2 texCoord;
out vec2 Texcoord;

void main()
{

   Texcoord = texCoord + texCoordOffset;
   gl_Position = proj * model * vec4(position.x, position.y, position.z, 1.f);
};