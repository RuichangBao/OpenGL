/*顶点着色器**/
#version 330 core
layout(location = 0) in vec3 aPos;      //顶点位置

uniform mat4 model;//模型矩阵	
uniform mat4 view;//观察矩阵
uniform mat4 projection;//投影矩阵

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}