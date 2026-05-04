#version 330 core
//the position variable is specified to have its index at 0
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
out vec3 vertexColor; // specify a color output to the fragment shader

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    vertexColor = aColor; 
}