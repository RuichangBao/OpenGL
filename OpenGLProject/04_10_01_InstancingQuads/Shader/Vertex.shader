/*默认正方体顶点着色器**/
#version 330 core
layout(location = 0) in vec2 aPos;      //顶点位置
layout(location = 1) in vec3 aColor;    //颜色

uniform mat4 model;//模型矩阵	
uniform mat4 view;//观察矩阵
uniform mat4 projection;//投影矩阵
uniform vec2 offsets[100];
out vec3 fColor;

void main()
{
    fColor = aColor;
    // fColor = vec3(gl_InstanceID/100.0f, gl_InstanceID/100.0f, gl_InstanceID/100.0f);
    fColor = vec3(gl_InstanceID/100.0f, 0, 0);
    vec2 offset = offsets[gl_InstanceID];
    //gl_Position = projection * view * model * vec4(aPos + offset, 0.0, 1.0);
    gl_Position = vec4(aPos + offset, 0.0, 1.0);
}