#version 330 core
layout(location = 0) in vec3 aPos;      //顶点位置
layout(location = 1) in vec3 aNormal;   //法线

out vec3 normals; //视图空间法线

uniform mat4 model;//模型矩阵	
uniform mat4 view;//观察矩阵

void main()
{
    //inverse 逆矩阵
    //view * model 模型 -> 世界 -> 观察
    //inverse(view * model)
    //transpose 转置矩阵
    //normalMatrix 将法线从模型空间变换到视图空间的矩阵
    mat3 normalMatrix = mat3(transpose(inverse(view * model)));
    normals = vec3(vec4(normalMatrix * aNormal, 0));
    gl_Position = view * model * vec4(aPos, 1.0); 
}