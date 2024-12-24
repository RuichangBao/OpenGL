/*光照正方体顶点着色器**/
#version 330 core
layout (location = 0)in vec3 aPos;
layout (location = 1)in vec3 aNormal;//模型空间下的法向量

uniform mat4 model;//模型矩阵	
uniform mat4 view;//观察矩阵
uniform mat4 projection;//投影矩阵

uniform vec3 viewPos;//观察者位置

out vec3 worldPos;//世界空间下的坐标
out vec3 Normal;

void main()
{
    worldPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
