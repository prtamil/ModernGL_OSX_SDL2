#version 330
layout(location=0) in vec2 position;
layout(location=1) in vec4 color;

out vec4 p;
smooth out vec4 theColor;

void main()
{
   gl_Position = vec4(position,0.0, 1.0);
   p = vec4(position,0.0, 1.0);
   theColor    = color;
}
