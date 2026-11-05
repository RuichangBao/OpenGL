/*顶点着色器**/
#version 330 core
layout(location = 0) in vec3 aPos;      //顶点位置
layout(location = 1) in vec3 aNormal;   //法线
layout(location = 2) in vec2 aTexCoords;//

uniform mat4 model;//模型矩阵	
uniform mat4 view;//观察矩阵
uniform mat4 projection;//投影矩阵

out VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
}vs_out;

void main()
{
    vs_out.FragPos = aPos;
    vs_out.Normal = aNormal;
    vs_out.TexCoords = aTexCoords;
    // gl_Position = projection * view * model * vec4(aPos, 1.0);
    gl_Position = projection * view  * vec4(aPos, 1.0);
}