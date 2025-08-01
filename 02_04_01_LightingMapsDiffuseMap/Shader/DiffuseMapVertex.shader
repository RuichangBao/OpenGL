/*漫反射光照贴图顶点着色器**/
#version 330 core
layout (location = 0)in vec3 aPos;
layout (location = 1)in vec3 aNormal;//模型空间下的法向量
layout (location = 2)in vec2 aTexCoords;//纹理坐标

uniform mat4 model;//模型矩阵	
uniform mat4 view;//观察矩阵
uniform mat4 projection;//投影矩阵

out vec3 worldPos;//世界空间下的坐标
out vec3 Normal;
out vec2 TexCoords;//纹理坐标

void main()
{
    worldPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoords = aTexCoords;
}
