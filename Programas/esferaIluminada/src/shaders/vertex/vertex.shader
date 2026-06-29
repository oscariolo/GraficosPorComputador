#version 330 core
//the position variable is specified to have its index at 0
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;

uniform mat4 model = mat4(1.0);
out vec3 ourColor;


void main()
{

    gl_Position = model * vec4(aPos, 1.0);
    ourColor = aColor;

}