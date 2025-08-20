/*顶点着色器**/
#version 330 core
layout(location = 0) in vec3 aPos;      //顶点位置(世界空间位置)

uniform mat4 model;

void main()
{
    gl_Position = model * vec4(aPos, 1.0);
}