/*默认正方体顶点着色器**/
#version 330 core
layout(location = 0) in vec3 aPos;      //顶点位置
// layout(location = 1) in vec3 aColor;    //颜色
layout(location = 2) in vec2 aTexCoords;   

uniform mat4 model;//模型矩阵	
uniform mat4 view;//观察矩阵
uniform mat4 projection;//投影矩阵
// uniform vec2 offsets[100];
out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
}