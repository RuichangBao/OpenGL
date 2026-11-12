/*默认正方体顶点着色器**/
#version 330 core
layout(location = 0) in vec3 aPos;      //顶点位置
layout(location = 2) in vec2 aTexCoords;   
layout(location = 3) in mat4 instanceMatrix;

//uniform mat4 model;//模型矩阵
uniform mat4 view;//观察矩阵
uniform mat4 projection;//投影矩阵

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = projection * view * instanceMatrix * vec4(aPos, 1.0f);
}