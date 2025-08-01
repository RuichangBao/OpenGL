// #version 330 core
// layout (location = 0) in vec3 aPos;

// out vec3 TexCoords;

// uniform mat4 projection;
// uniform mat4 view;

// void main()
// {
//     TexCoords = aPos;
//     gl_Position = projection * view * vec4(aPos, 1.0);
// }

#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;    
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;//深度是根据z/w 得到的  把z设置为w 得到的深度始终为1
    //gl_Position = vec4(pos.x, pos.y, pos.z - 0.01 , pos.z);
}