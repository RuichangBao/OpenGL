/*顶点着色器**/
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

//模型空间下的计算
out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 ModelLightPos;
    vec3 ModelViewPos;
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
    //模型空间下的计算
    vs_out.ModelLightPos = vec3(model * vec4(lightPos, 1.0));
    vs_out.ModelViewPos = vec3(model * vec4(viewPos, 1.0));
        
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}