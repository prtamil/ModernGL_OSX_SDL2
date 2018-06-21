#version 330
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

smooth out vec4 theColor;

void main()
{
   gl_Position = projMatrix * viewMatrix * modelMatrix  * position;
   theColor    = color;
}
