#version 330
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

uniform ivec2 offset;

smooth out vec4 theColor;

void main()
{
   gl_Position = vec4(offset.x/100.0,offset.y/100.0,0.0,1.5) +  position;
   theColor    = color;
}
