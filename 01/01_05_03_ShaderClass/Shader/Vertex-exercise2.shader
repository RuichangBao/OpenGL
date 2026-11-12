/*顶点着色器 向左偏移
**/
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;
uniform float xOffset; // 在OpenGL程序代码中设定这个变量

void main()
{
    gl_Position = vec4(aPos.x - xOffset, aPos.y, aPos.z, 1.0);//三角形上线颠倒
    ourColor = aColor;
}