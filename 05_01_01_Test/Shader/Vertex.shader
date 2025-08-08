/*顶点着色器**/
#version 330 core
layout(location = 0) in vec2 aPos;      //顶点位置
out vec4 FragColor;

uniform mat4 model;//模型矩阵	
uniform mat4 view;//观察矩阵
uniform mat4 projection;//投影矩阵

void main()
{
    // gl_Position = projection * view  * vec4(aPos, 1.0);
    FragColor = vec4((aPos.x+0.9)/1.8, (aPos.x+0.9)/1.8, (aPos.x+0.9)/1.8, 1);
    gl_Position = vec4(aPos, 0.0, 1.0);
}