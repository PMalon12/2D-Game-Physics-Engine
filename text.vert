#version 150 core

uniform vec2 screenSize;

in vec2 position;
in vec4 color;
in vec2 texcoord;
out vec4 Color;
out vec2 Texcoord;
void main() {
   Color = color;
   Texcoord = texcoord;
   gl_Position = vec4(position.x - 1.0f, 1.0f + position.y, 0, 1.0);
};