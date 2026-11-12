/*默认正方体顶点着色器**/
#version 330 core
layout(location = 0) in vec3 aPos;      //顶点位置
layout(location = 1) in vec3 aNormal;   //法线
layout(location = 2) in vec2 aTexCoords;//贴图

out vec2 texCoords;

uniform mat4 model;//模型矩阵	
uniform mat4 view;//观察矩阵
uniform mat4 projection;//投影矩阵

void main()
{
    texCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}