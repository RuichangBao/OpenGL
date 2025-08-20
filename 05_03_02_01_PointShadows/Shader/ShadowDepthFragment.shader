/*片段着色器**/
#version 330 core

in vec4 FragPos;

uniform vec3 lightPos;
uniform float far_plane;

void main()
{
    //计算距离  自己计算深度值 并且映射到0-1
    float lightDistance = length(FragPos.xyz - lightPos);
 
    // 映射到[0-1]范围除以far_plane
    lightDistance = lightDistance / far_plane;
    
    // 将此写入修改后的深度
    gl_FragDepth = lightDistance;
}