#version 330 core
layout (location = 0) in vec3 aPos;     //模型位置
layout (location = 1) in vec3 aNormal;  //模型法线

out vec3 Normal;     //法线
out vec3 Position;   //世界位置

uniform mat4 model; //模型空间变换到世界空间的矩阵
uniform mat4 view;
uniform mat4 projection;

void main()
{
    //mat3(transpose(inverse(model))) 模型矩阵的逆矩阵的转置，用来把法线从模型空间变换到世界空间
    Normal = mat3(transpose(inverse(model))) * aNormal;
    Position = vec3(model * vec4(aPos, 1.0));    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    gl_PointSize = 0.001f;
}
