#version 330 core
//the position variable is specified to have its index at 0
layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos, 1.0);
}