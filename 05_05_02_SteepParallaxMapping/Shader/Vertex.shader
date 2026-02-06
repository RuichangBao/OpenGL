/*顶点着色器**/
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;


//切线空间下的计算
out VS_OUT {
    vec3 FragPos;           //世界空间位置
    vec2 TexCoords;
    vec3 TangentLightPos;   //切线空间光源位置
    vec3 TangentViewPos;    //切线空间视角方向
    vec3 TangentFragPos;    //切线空间物体位置
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));   
    vs_out.TexCoords = aTexCoords;


    //切线空间下的计算
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * aTangent);//得到世界空间（或观察空间）下的切线方向
    vec3 N = normalize(normalMatrix * aNormal); //得到世界空间（或观察空间）下的法线方向。
    T = normalize(T - dot(T, N) * N);   //修正因变换或模型数据不完美导致的T与N不垂直问题，构造出标准的正交基。
    vec3 B = cross(N, T);
    
    mat3 TBN = transpose(mat3(T, B, N));    
    vs_out.TangentLightPos = TBN * lightPos;
    vs_out.TangentViewPos  = TBN * viewPos;
    vs_out.TangentFragPos  = TBN * vs_out.FragPos;
        
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}