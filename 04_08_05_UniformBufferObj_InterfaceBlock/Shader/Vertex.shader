#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

// out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//结构体名称要相同，声明的结构体变量名称可以不同
out VertexData {
    vec2 texCoord;
} vData;

void main()
{
    vData.texCoord = aTexCoords;    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}