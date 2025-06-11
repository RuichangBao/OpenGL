/*默认正方体顶点着色器**/
#version 330 core
layout(location = 0) in vec3 aPos;      //顶点位置
layout(location = 1) in vec2 aTexCoords;//贴图

uniform mat4 model;//模型矩阵	
uniform mat4 view;//观察矩阵
uniform mat4 projection;//投影矩阵

out VS_OUT {
    vec2 TexCoords;
    vec3 Pos;
} gs_in;

void main()
{
    gs_in.Pos = aPos;
    gs_in.TexCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}