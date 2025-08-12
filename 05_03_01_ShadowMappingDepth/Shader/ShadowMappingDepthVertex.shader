/*顶点着色器**/
#version 330 core
layout(location = 0) in vec3 aPos;      //顶点位置

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
}