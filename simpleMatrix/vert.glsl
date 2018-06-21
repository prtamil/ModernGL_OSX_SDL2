#version 330
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

uniform mat4 worldMatrix;

smooth out vec4 theColor;

void main()
{
   gl_Position = cos(position.x) *worldMatrix * position;
   theColor    = color;
}
